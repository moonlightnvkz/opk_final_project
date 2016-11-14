//
// Created by moonlightnvkz on 09.11.16.
//

#include "server_logic.h"
#include<stdio.h> //printf
#include <string.h>

int recieve_reply_from_the_server(int *sock, char *reply, size_t size)
{
    if(recv(*sock , reply , 2000 , 0) < 0) {
        puts("recv failed");
        return 1;
    }
    return 0;
}

int send_data_to_the_server(int *sock, char *message)
{
    if(send(*sock , message , strlen(message) , 0) < 0) {
        puts("Send failed");
        return 1;
    }
    return 0;
}

int connect_to_server(int *sock, struct sockaddr_in *server)
{
    //Create socket
    *sock = socket(AF_INET , SOCK_STREAM , 0);
    if (*sock == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    server->sin_addr.s_addr = inet_addr("127.0.0.1");
    server->sin_family = AF_INET;
    server->sin_port = htons( 8888 );

    //Connect to remote server
    if (connect(*sock , (struct sockaddr *)server , sizeof(*server)) < 0) {
        perror("Connect failed. Error");
        return 1;
    }

    puts("Connected\n");
    return 0;
}