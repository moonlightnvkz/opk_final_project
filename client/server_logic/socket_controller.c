//
// Created by moonlightnvkz on 19.11.16.
//

#include <stdio.h>
#include <unistd.h>    //close
#include <SDL_timer.h>
#include "socket_controller.h"
#include "../default_values.h"
#include "../game_logic/player.h"
#include "../loggers.h"

int sc_connect_to_server(SocketController *sc);

int sc_send_data_to_the_server(SocketController *sc, RequestStructure *request, size_t size);

int sc_receive_reply_from_the_server(SocketController *sc, ResponseStructure *reply, size_t size);

SocketController *sc_init()
{
    SocketController *sc = malloc(sizeof(SocketController));
    if (sc == NULL) {
        log_error("Failed to allocate memory for socket controller", __FUNCTION__, __LINE__);
        return NULL;
    }

    deque_create(&sc->requests_list);

    if (sc_connect_to_server(sc) != 0) {
        log_error("Failed to connect to the server", __FUNCTION__, __LINE__);
        sc_destroy(sc);
        return NULL;
    }

    sc->last_response.res_number = 0;
    sc->last_response.this_player_state.position.x = sc->last_response.this_player_state.position.y = 0;
    sc->last_response.this_player_state.angle = 0;
    return sc;
}
void sc_destroy(SocketController *sc)
{
    if (shutdown(sc->sock, 2) == -1) {
        log_error("Failed to close the socket", __FUNCTION__, __LINE__);
    };
    close(sc->sock);
    deque_destroy(&sc->requests_list);
    free(sc);
}

int sc_connect_to_server(SocketController *sc) {
    //Create socket
    sc->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sc->sock == -1) {
        log_error("Failed to create socket", __FUNCTION__, __LINE__);
        return SC_SOCKET_CREATION_ERROR;
    }

    sc->server.sin_addr.s_addr = inet_addr(SERVER_IP);
    sc->server.sin_family = AF_INET;
    sc->server.sin_port = htons(SERVER_PORT);

    //Connect to remote server
    if (connect(sc->sock, (struct sockaddr *) &sc->server, sizeof(sc->server)) < 0) {
        log_error("Failed to connect to the server", __FUNCTION__, __LINE__);
        return SC_CONNECTION_FAILED;
    }

    log_action("Connected", __FUNCTION__, __LINE__);
    return SC_NO_ERROR;
}

int sc_send_current_state(SocketController *sc, Player *player)
{
    static unsigned last_send_time = 0;
    if (!player->shot_done && last_send_time < 1 / SERVER_TICKRATE * 1000) {
        return SC_NO_ERROR;
    }
    RequestStructure *peeked = ((RequestStructure *) deque_peek_last(&sc->requests_list));
    unsigned last_number;
    if (peeked == NULL) {
        last_number = 0;
    } else {
        last_number = peeked->req_number;
    }
    RequestStructure *req = request_create(player, last_number + 1);
    deque_add_last(&sc->requests_list, (Pointer) req);

    last_send_time = SDL_GetTicks();
    if (send(sc->sock, req, sizeof(RequestStructure), MSG_DONTWAIT) < 0) {
        log_error("Send failed", __FUNCTION__, __LINE__);
        return SC_SEND_FAILED;
    }
    request_log(req, "Send", __FUNCTION__, __LINE__);
    return SC_NO_ERROR;
}

// @return:
// 0 - all is Ok
// 1 - failed to receive
int sc_receive_current_state(SocketController *sc) {
    static ssize_t left_to_receive = sizeof(ResponseStructure);
    ssize_t res = recv(sc->sock, &sc->last_response, sizeof(ResponseStructure), MSG_DONTWAIT);
    if (res == -1) {
        log_error("Receive failed", __FUNCTION__, __LINE__);
        return SC_RECEIVE_FAILED;
    }
    if (res == 0) {
        log_error("Connection closed", __FUNCTION__, __LINE__);
        return SC_CONNECTION_CLOSED;
    }
    left_to_receive -= res;
    // <<<<<<<<<<< FOR DEBUG <<<<<<<<<<<<<<<<<<
    if (left_to_receive < 0) {
        log_error("Left to receive is bellow zero", __FUNCTION__, __LINE__);
        return SC_RECEIVE_FAILED;
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    if (left_to_receive > 0) {
        log_error("<<<<<<<<<<<<<<<<<<<<<", __FUNCTION__, __LINE__);
        return SC_PARTIAL_RECEIVE;
    }
    left_to_receive = sizeof(ResponseStructure);
    response_log(&sc->last_response, "Recv", __FUNCTION__, __LINE__);
    return SC_NO_ERROR;
}

int sc_receive_start_signal(SocketController *sc)
{
    char start_signal = 0;
    if (recv(sc->sock, &start_signal, SERVER_START_SIGNAL_LENGTH, MSG_WAITALL) <= 0) {
        log_error("Receive of start signal failed", __FUNCTION__, __LINE__);
        return SC_RECEIVE_FAILED;
    }
    if (start_signal != SERVER_START_SIGNAL) {
        return SC_START_SIGNAL_MATHCHING_ERROR;
    }
    char msg[50];
    sprintf(msg, "Start signal received:%c", start_signal);
    log_action("Start signal received", __FUNCTION__, __LINE__);
    return SC_NO_ERROR;
}
