#include <arpa/inet.h> //inet_addr
#include <SDL_timer.h>
#include "server_logic/request_response.h"
#include "loggers.h"
#include "game_logic/model_controller.h"
#include "server_logic/socket_controller.h"
#include "globals.h"

int main(int argc , char *argv[])
{
    if (!logger_init()) {
        LOG_ERROR("Failed to initialize logger");
        return 1;
    }
    if (!globals_init()) {
        LOG_ERROR("Failed to initialize global variables");
        logger_destroy();
        return 1;
    }
    ModelController mc;
    if (!mc_init(&mc)) {
        LOG_ERROR("Failed to initialize model controller");
        logger_destroy();
        return 1;
    }
    SocketController sc;
    if (!sc_init(&sc)) {
        LOG_ERROR("Failed to initialize socket controller");
        mc_destroy(&mc);
        logger_destroy();
        return 1;
    }
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        if (sc_accept_player(&sc, i) != SC_NO_ERROR) {
            mc_destroy(&mc);
            sc_destroy(&sc);
            LOG_ERROR("Failed to accept player connection");
            logger_destroy();
            return 0;
        }
    }
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        if (sc_send_start_signal(&sc, i) != SC_NO_ERROR) {
            mc_destroy(&mc);
            sc_destroy(&sc);
            LOG_ERROR("Failed to send start signal");
            logger_destroy();
            return 0;
        }
    }

    unsigned curr_ticks = 0, prev_ticks = 0;
    bool quit = false;
    int results_of_receive[PLAYER_COUNT] = {0};
    unsigned time = 0;
    while(true) {
        for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
            results_of_receive[i] = sc_receive_request(&sc, i);
            if (results_of_receive[i] == SC_CONNECTION_CLOSED || results_of_receive[i] == SC_UNSUPPORTED_PLAYER) {
                quit = true;
                break;
            }
            if (results_of_receive[i] == SC_NO_ERROR) {
                request_log(&sc.request, "Request", __FUNCTION__, __LINE__);
                mc_apply_request(&mc, i, &sc.request);
            }
        }

        sc_create_responses(&sc, &mc, quit);
        response_log(&sc.response, "Response", __FUNCTION__, __LINE__);

        for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
            if (results_of_receive[i] == SC_NO_ERROR || SDL_GetTicks() - time > 1000.0 / SERVER_TICKRATE) {
                sc_send_response(&sc, i);
            }
        }
        if (SDL_GetTicks() - time > 1000.0 / SERVER_TICKRATE) {
            time = SDL_GetTicks();
        }
        if (quit) { // quit should be there because of sc_create_response(..., ..., quit);
            break;
        }
        mc_process_moving(&mc, curr_ticks - prev_ticks);
        if (mc_alive_players_count(&mc) <= GlobalVariables.players_alive_to_end_the_game) {
            quit = true;
        }
        prev_ticks = curr_ticks;
        curr_ticks = SDL_GetTicks();
    }

    mc_destroy(&mc);
    sc_destroy(&sc);
    logger_destroy();
    return 0;
}