//
// Created by moonlightnvkz on 09.11.16.
//
#pragma once
#include <stdlib.h>
#include <arpa/inet.h> //inet_addr

// 1 = connection failed
int connect_to_server(int *sock, struct sockaddr_in *server);

int send_data_to_the_server(int *sock, char *message);

int recieve_reply_from_the_server(int *sock, char *reply, size_t size);