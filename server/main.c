/*
    C socket server example
*/

#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <SDL_timer.h>
#include "server_logic/response_request.h"
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
        if (res == 1) {
            quit = true;
        }
        if (res != 2) {
            request_log(&sc->request_player, "Request ", __FUNCTION__, __LINE__);
            mc_apply_request(mc, mc->player1, &sc->request_player);
            mc_create_response(mc, 1, sc->request_player.req_number, &sc->response_player);
            response_log(&sc->response_player, "Response", __FUNCTION__, __LINE__);
            sc_send_response(sc, 1);
        }
        res = sc_receive_request(sc, 2);
        if (res == 1) {
            quit = true;
        }
        if (res != 2) {
            request_log(&sc->request_player, "Request ", __FUNCTION__, __LINE__);
            mc_apply_request(mc, mc->player2, &sc->request_player);
            mc_create_response(mc, 2, sc->request_player.req_number, &sc->response_player);
            response_log(&sc->response_player, "Response", __FUNCTION__, __LINE__);
            sc_send_response(sc, 2);
        }
        mc_process_moving(mc, curr_ticks - prev_ticks);
        prev_ticks = curr_ticks;
        curr_ticks = SDL_GetTicks();
    }

    mc_destroy(mc);
    sc_destroy(sc);
    logger_destroy();
    return 0;
}