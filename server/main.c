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
    ModelController *mc = mc_init();
    if (mc == NULL) {
        log_error("Failed to initialize model controller", __FUNCTION__, __LINE__);
        logger_destroy();
        exit(1);
    }
    SocketController *sc = sc_init();
    if (sc == NULL) {
        log_error("Failed to initialize socket controller", __FUNCTION__, __LINE__);
        mc_destroy(mc);
        logger_destroy();
        exit(1);
    }
    sc_accept_player(sc, 1);
    sc_accept_player(sc, 2);

    sc_send_start_signal(sc, 1);
    sc_send_start_signal(sc, 2);

    unsigned curr_ticks = 0, prev_ticks = 0;
    bool quit = false;
    while(!quit) {
        int res;
        res = sc_receive_request(sc, 1);
        if (res == SC_CONNECTION_CLOSED || res == SC_UNSUPPORTED_PLAYER) {
            break;
        }
        if (res == SC_NO_ERROR) {
            request_log(&sc->request_player1, "Request 1", __FUNCTION__, __LINE__);
            mc_apply_request(mc, mc->player1, &sc->request_player1);
        }

        res = sc_receive_request(sc, 2);
        if (res == SC_CONNECTION_CLOSED || res == SC_UNSUPPORTED_PLAYER) {
            break;
        }
        if (res == SC_NO_ERROR) {
            request_log(&sc->request_player2, "Request 2", __FUNCTION__, __LINE__);
            mc_apply_request(mc, mc->player2, &sc->request_player2);
        }
        sc_create_responses(sc, mc);
        response_log(&sc->response_player1, "Response 1", __FUNCTION__, __LINE__);
        response_log(&sc->response_player2, "Response 2", __FUNCTION__, __LINE__);
        sc_send_response(sc, 1);
        sc_send_response(sc, 2);

        mc_reset_temp_flags(mc);
        mc_process_moving(mc, curr_ticks - prev_ticks);
        prev_ticks = curr_ticks;
        curr_ticks = SDL_GetTicks();
    }

    mc_destroy(mc);
    sc_destroy(sc);
    logger_destroy();
    return 0;
}