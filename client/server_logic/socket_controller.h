//
// Created by moonlightnvkz on 19.11.16.
//

#pragma once
#include<arpa/inet.h> //inet_addr
#include "../my_deque.h"
#include "response_request.h"

typedef struct tPlayer Player;

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
// 2 - reply might be wrong
// 3 - changes need to be applied
// 4 - dequeued == NULL
int sc_receive_current_state(SocketController *sc);

//int sc_connect_to_server(SocketController *sc);

//int sc_send_data_to_the_server(SocketController *sc, char *message, size_t length);

//int sc_receive_reply_from_the_server(SocketController *sc, char *reply, size_t size);