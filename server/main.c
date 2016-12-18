/*
    C socket server example
*/

#include <arpa/inet.h> //inet_addr
#include <SDL_timer.h>
#include "server_logic/request_response.h"
#include "loggers.h"
#include "game_logic/model_controller.h"
#include "server_logic/socket_controller.h"


int main(int argc , char *argv[])
{
    logger_init();
    globals_init();
    ModelController mc;
    if (!mc_init(&mc)) {
        log_error("Failed to initialize model controller", __FUNCTION__, __LINE__);
        logger_destroy();
        exit(1);
    }
    SocketController sc;
    if (!sc_init(&sc)) {
        log_error("Failed to initialize socket controller", __FUNCTION__, __LINE__);
        mc_destroy(&mc);
        logger_destroy();
        exit(1);
    }
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        if (sc_accept_player(&sc, i) != SC_NO_ERROR) {
            mc_destroy(&mc);
            sc_destroy(&sc);
            log_error("Failed to accept player connection", __FUNCTION__, __LINE__);
            logger_destroy();
            return 0;
        }
    }
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        if (sc_send_start_signal(&sc, i) != SC_NO_ERROR) {
            mc_destroy(&mc);
            sc_destroy(&sc);
            log_error("Failed to send start signal", __FUNCTION__, __LINE__);
            logger_destroy();
            return 0;
        }
    }

    unsigned curr_ticks = 0, prev_ticks = 0;
    bool quit = false;
    while(!quit) {
        for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
            int res;
            res = sc_receive_request(&sc, i);
            if (res == SC_CONNECTION_CLOSED || res == SC_UNSUPPORTED_PLAYER) {
                quit = true;
                break;
            }
            if (res == SC_NO_ERROR) {
                request_log(&sc.request, "Request", __FUNCTION__, __LINE__);
                mc_apply_request(&mc, i, &sc.request);

                sc_create_responses(&sc, &mc);
                sc_send_response(&sc, i);
                response_log(&sc.response, "Response", __FUNCTION__, __LINE__);
            }
        }
        mc_process_moving(&mc, curr_ticks - prev_ticks);
        prev_ticks = curr_ticks;
        curr_ticks = SDL_GetTicks();
    }

    mc_destroy(&mc);
    sc_destroy(&sc);
    logger_destroy();
    return 0;
}