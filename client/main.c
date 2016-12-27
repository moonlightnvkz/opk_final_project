#include <arpa/inet.h> //inet_addr
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "loggers.h"
#include "game_logic/mvc.h"
#include "server_logic/socket_controller.h"
#include "globals.h"

// TODO: UI

// TODO: bullets reflection on static objects

// TODO: big boom

// TODO: serialization / deserialization

int main(int argc , char *argv[])
{
    if (!logger_init()) {
        return 1;
    }
    if(!globals_init()) {
        LOG_ERROR("Failed to initialize global variables");
        logger_destroy();
        return 2;
    }
    MVC mvc;
    if (!mvc_init(&mvc)) {
        LOG_ERROR("Failed to create mvc");
        logger_destroy();
        return 3;
    }
    SocketController socketController;
    if (!sc_init(&socketController)) {
        LOG_ERROR("Failed to create socket controller");
        mvc_destroy(&mvc);
        logger_destroy();
        return 4;
    }

    // render waiting text
    SDL_RenderClear(mvc.renderer);
    text_render_text(&mvc.text, TEXT_WAITING_FOR_PLAYERS, mvc.renderer, 0, 255, 0);
    SDL_RenderPresent(mvc.renderer);

    if (sc_receive_start_signal(&socketController) != SC_NO_ERROR) {
        LOG_ERROR("sc_receive_start_signal failed");
        mvc_destroy(&mvc);
        sc_destroy(&socketController);
        logger_destroy();
        return 3;
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

        //unsigned time = SDL_GetTicks();
        sc_send_current_state(&socketController, &mvc);
        //LOG_ACTION("send:%d", SDL_GetTicks() - time);

        //time = SDL_GetTicks();
        res = sc_receive_current_state(&socketController);
        //LOG_ACTION("recv:%d", SDL_GetTicks() - time);

        if (res == SC_CONNECTION_CLOSED) {
            LOG_ACTION(Connection closed);
            break;
        }
        if (res == SC_NO_ERROR) {
            //time = SDL_GetTicks();
            mvc_apply_response(&mvc, &socketController.requests_list, &socketController.last_response);
            //LOG_ACTION("appl:%d", SDL_GetTicks() - time);
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