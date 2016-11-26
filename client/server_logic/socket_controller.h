//
// Created by moonlightnvkz on 19.11.16.
//

#pragma once
#include <arpa/inet.h> //inet_addr
#include "../my_deque.h"
#include "request_response.h"

typedef struct tPlayer Player;

typedef enum eScReturnCodes {
    SC_NO_ERROR,
    SC_RECEIVE_FAILED,
    SC_SEND_FAILED,
    SC_CONNECTION_CLOSED,
    SC_CONNECTION_FAILED,
    SC_START_SIGNAL_MATHCHING_ERROR,
    SC_SOCKET_CREATION_ERROR
} ScReturnCodes;

typedef struct tSocketController {
    int sock;
    struct sockaddr_in server;
    Deque requests_list;
    ResponseStructure last_response;
} SocketController;

SocketController *sc_init();

void sc_destroy(SocketController *sc);

int sc_send_current_state(SocketController *sc, Player *player);

// @return:
// 0 - all is Ok
// 1 - failed to receive
int sc_receive_current_state(SocketController *sc);

int sc_receive_start_signal(SocketController *sc);