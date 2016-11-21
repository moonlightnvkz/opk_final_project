//
// Created by moonlightnvkz on 20.11.16.
//

#pragma once

#include<arpa/inet.h> //inet_addr
#include "response_request.h"

typedef struct tSocketController {
    int socket_desc;
    int socket_player1;
    int socket_player2;
    struct sockaddr_in server;
    RequestStructure request_player;
    ResponseStructure response_player;
} SocketController;

SocketController *sc_init();

void sc_destroy(SocketController *sc);

int sc_accept_player(SocketController *sc, unsigned number);

int sc_receive_request(SocketController *sc, unsigned number_of_player);

int sc_send_response(SocketController *sc, unsigned number_of_player);

int sc_send_start_signal(SocketController *sc, unsigned number_of_player);