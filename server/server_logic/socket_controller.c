//
// Created by moonlightnvkz on 20.11.16.
//

#include <malloc.h>
#include <sys/socket.h>
#include <unistd.h>    //write
#include "socket_controller.h"
#include "../loggers.h"
#include "../default_values.h"

SocketController *sc_init()
{
    SocketController *sc = malloc(sizeof(SocketController));
    if (sc == NULL) {
        log_error("Failed to allocate memory for socket controller", __FUNCTION__, __LINE__);
        return NULL;
    }
    sc->socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (sc->socket_desc == -1) {
        log_error("Failed to create socket", __FUNCTION__, __LINE__);
        return NULL;
    }
    sc->server.sin_family = AF_INET;
    sc->server.sin_addr.s_addr = INADDR_ANY;
    sc->server.sin_port = htons(SERVER_PORT);

    if (bind(sc->socket_desc, (struct sockaddr *) &sc->server, sizeof(sc->server)) < 0) {
        log_error("Failed to bind", __FUNCTION__, __LINE__);
        return NULL;
    }

    listen(sc->socket_desc , 3);

    char msg[50];
    sprintf(msg, "Server listen at %d:%d", INADDR_ANY, SERVER_PORT);
    log_action(msg, __FUNCTION__, __LINE__);
}

void sc_destroy(SocketController *sc)
{
    close(sc->socket_player1);
    close(sc->socket_player2);
    close(sc->socket_desc);
    free(sc);
}

int *get_player_socket_by_number(SocketController *sc, int number)
{
    switch (number) {
        case 1:
            return &sc->socket_player1;
        case 2:
            return &sc->socket_player2;
        default:
            return NULL;
    }
}

int sc_accept_player(SocketController *sc, unsigned number)
{
    int *player_socket = get_player_socket_by_number(sc, number);
    if (player_socket == NULL) {
        log_error("Unsupported player", __FUNCTION__, __LINE__);
        return 1;
    }
    char msg[50];
    sprintf(msg, "Waiting for incoming connection: player %d", number);
    log_action(msg, __FUNCTION__, __LINE__);
    *player_socket = accept(sc->socket_desc, NULL, NULL);
    if (*player_socket < 0) {
        log_error("Failed to accept player connection", __FUNCTION__, __LINE__);
        return 1;
    }
    sprintf(msg, "Connection accepted:player %d", number);
    log_action(msg, __FUNCTION__, __LINE__);
    return 0;
}

int sc_receive_player(SocketController *sc, unsigned number)
{
    int *player_socket = get_player_socket_by_number(sc, number);
    if (player_socket == NULL) {
        log_error("Unsupported player", __FUNCTION__, __LINE__);
        return 1;
    }
    ssize_t read_size = recv(*player_socket , &sc->request_player , sizeof(RequestStructure) , 0);
    if(read_size == 0) {
        char msg[50];
        sprintf(msg, "Player %d disconnected", number);
        log_action(msg, __FUNCTION__, __LINE__);
        fflush(stdout);
        return 1;
    }
    else if(read_size == -1) {
        log_error("Receive failed", __FUNCTION__, __LINE__);
        return 2;
    }
    return 0;
}