#include <arpa/inet.h> //inet_addr
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "loggers.h"
#include "game_logic/mvc.h"
#include "server_logic/socket_controller.h"

// TODO: UI

// TODO: bullets reflection on static objects

// TODO: will not be done: camera(player) -> camera(mouse) - need to warp mouse to the center after each loop...

// TODO: big boom

// TODO: serialization / deserialization

int main(int argc , char *argv[])
{
    logger_init();
    globals_init();
    MVC mvc;
    if (!mvc_init(&mvc)) {
        LOG_ERROR("Failed to create mvc");
        logger_destroy();
        exit(2);
    }
    SocketController socketController;
    if (!sc_init(&socketController)) {
        LOG_ERROR("Failed to create socket controller");
        mvc_destroy(&mvc);
        logger_destroy();
        exit(2);
    }
    SDL_RenderClear(mvc.renderer);
    text_render_text(&mvc.text, TEXT_WAITING_FOR_PLAYERS, mvc.renderer, 0, 255, 0);
    SDL_RenderPresent(mvc.renderer);

    if (sc_receive_start_signal(&socketController) != SC_NO_ERROR) {
        LOG_ERROR("sc_receive_start_signal failed");
        mvc_destroy(&mvc);
        sc_destroy(&socketController);
        logger_destroy();
    }
    text_free_last_text(&mvc.text);

    unsigned curr_ticks = 0, prev_ticks = 0;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    while (!GlobalVariables.quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
                case SDL_QUIT:
                    GlobalVariables.quit = true;
                    break;
                default:;
            }
        }
        int res;
        res = mvc_process_key(&mvc, keystates);
        if (res == MVC_EXIT_KEY_PRESSED) {
            break;
        }

        mvc_process_moving(&mvc, curr_ticks - prev_ticks);

        sc_send_current_state(&socketController, &mvc);

        res = sc_receive_current_state(&socketController);
        if (res == SC_CONNECTION_CLOSED) {
            LOG_ACTION(Connection closed);
            break;
        }
        if (res == SC_NO_ERROR) {
            mvc_apply_response(&mvc, &socketController.requests_list, &socketController.last_response);
        }

        mvc_render(&mvc);

        prev_ticks = curr_ticks;
        curr_ticks = SDL_GetTicks();
    }

    mvc_destroy(&mvc);
    sc_destroy(&socketController);
    logger_destroy();
    return 0;
}