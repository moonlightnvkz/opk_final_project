//
// Created by moonlightnvkz on 20.11.16.
//

#pragma once

#include<arpa/inet.h> //inet_addr
#include "request_response.h"

typedef struct tModelController ModelController;

typedef enum eScErrors {
    SC_NO_ERROR,
    SC_RECEIVE_FAILED,
    SC_SEND_FAILED,
    SC_CONNECTION_CLOSED,
    SC_CONNECTION_FAILED,
    SC_START_SIGNAL_MATHCHING_ERROR,
    SC_SOCKET_CREATION_ERROR,
    SC_UNSUPPORTED_PLAYER
} ScErrors;

typedef struct tSocketController {
    int socket_desc;
    int socket_player1;
    int socket_player2;
    struct sockaddr_in server;
    RequestStructure request_player1;
    RequestStructure request_player2;
    ResponseStructure response_player1;
    ResponseStructure response_player2;
} SocketController;

SocketController *sc_init();

void sc_destroy(SocketController *sc);

int sc_accept_player(SocketController *sc, unsigned number);

int sc_receive_request(SocketController *sc, unsigned number_of_player);

void sc_create_responses(SocketController *sc, ModelController *mc);

int sc_send_response(SocketController *sc, unsigned player_number);

int sc_send_start_signal(SocketController *sc, unsigned number_of_player);