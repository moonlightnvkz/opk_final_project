#include <arpa/inet.h> //inet_addr
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "loggers.h"
#include "game_logic/mvc.h"
#include "server_logic/socket_controller.h"
#include "game_logic/player.h"

// TODO: collision - DONE: players are now limited in space to window borders
// TODO: kills
// TODO: bullet reflection  ???
// TODO: static items with collision
// TODO: big map with small view (DONE) + arrows pointing to other player(s)
// TODO: EXPLOSIONS!!! (barrels)

int main(int argc , char *argv[])
{
    logger_init();
    MVC *mvc = mvc_init();
    if (mvc == NULL) {
        log_error("Failed to create mvc", __FUNCTION__, __LINE__);
        logger_destroy();
        exit(1);
    }
    SocketController *socketController = sc_init();
    if (socketController == NULL) {
        log_error("Failed to create socket controller", __FUNCTION__, __LINE__);
        mvc_destroy(mvc);
        logger_destroy();
        exit(1);
    }
    if (sc_receive_start_signal(socketController) != SC_NO_ERROR) {
        log_error("sc_receive_start_signal failed", __FUNCTION__, __LINE__);
        mvc_destroy(mvc);
        sc_destroy(socketController);
        logger_destroy();
    }
    bool quit = false;
    unsigned curr_ticks = 0, prev_ticks = 0;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                default:;
            }
        }
        int res;
        res = mvc_process_key(mvc, keystates);
        if (res == MVC_EXIT_KEY_PRESSED) {
            break;
        }

        mvc_process_moving(mvc, curr_ticks - prev_ticks);

        sc_send_current_state(socketController, mvc->players[GlobalVariables.number_of_player]);

        res = sc_receive_current_state(socketController);
        if (res == SC_CONNECTION_CLOSED) {
            log_error("Connection closed", __FUNCTION__, __LINE__);
            break;
        }
        if (res == SC_NO_ERROR) {
            mvc_apply_response(mvc, &socketController->requests_list, &socketController->last_response);
        }

        mvc_render(mvc);

        prev_ticks = curr_ticks;
        curr_ticks = SDL_GetTicks();
    }

    mvc_destroy(mvc);
    sc_destroy(socketController);
    logger_destroy();
    return 0;
}