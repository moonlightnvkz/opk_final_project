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
    SC_UNSUPPORTED_PLAYER
} ScErrors;

typedef struct tSocketController {
    int server_socket;
    int player_sockets[PLAYER_COUNT];
    struct sockaddr_in server;
    unsigned request_numbers[PLAYER_COUNT]; // numbers of request for each player
    RequestStructure request;   // res_number should be set exactly before sending response!
    ResponseStructure response;
} SocketController;

bool sc_init(SocketController *sc);

void sc_destroy(SocketController *sc);

int sc_accept_player(SocketController *sc, unsigned number_of_player);

int sc_receive_request(SocketController *sc, unsigned number_of_player);

void sc_create_responses(SocketController *sc, ModelController *mc, bool quit);

int sc_send_response(SocketController *sc, unsigned number_of_player);

int sc_send_start_signal(SocketController *sc, unsigned number_of_player);