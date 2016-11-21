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
    return sc;
}

void sc_destroy(SocketController *sc)
{
    shutdown(sc->socket_player1, 2);
    shutdown(sc->socket_player2, 2);
    close(sc->socket_player1);
    close(sc->socket_player2);
    close(sc->socket_desc);
    free(sc);
}

int *get_player_socket_by_number(SocketController *sc, unsigned number)
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
        return SC_UNSUPPORTED_PLAYER;
    }
    char msg[50];
    sprintf(msg, "Waiting for incoming connection: player %d", number);
    log_action(msg, __FUNCTION__, __LINE__);
    *player_socket = accept(sc->socket_desc, NULL, NULL);
    if (*player_socket < 0) {
        log_error("Failed to accept player connection", __FUNCTION__, __LINE__);
        return SC_CONNECTION_FAILED;
    }
    sprintf(msg, "Connection accepted:player %d", number);
    log_action(msg, __FUNCTION__, __LINE__);
    return SC_NO_ERROR;
}

RequestStructure *get_player_request_by_number(SocketController *sc, unsigned number)
{
    switch (number) {
        case 1:
            return &sc->request_player1;
        case 2:
            return &sc->request_player2;
        default:
            return NULL;
    }
}

int sc_receive_request(SocketController *sc, unsigned number_of_player)
{
    if (number_of_player != 1 && number_of_player != 2) {
        log_error("Unsupported player", __FUNCTION__, __LINE__);
        return SC_UNSUPPORTED_PLAYER;
    }
    int *player_socket = get_player_socket_by_number(sc, number_of_player);
    RequestStructure *player_request = get_player_request_by_number(sc, number_of_player);
    ssize_t read_size = recv(*player_socket , player_request , sizeof(RequestStructure) , 0);
    if(read_size == 0) {
        char msg[50];
        sprintf(msg, "Player %d disconnected", number_of_player);
        log_action(msg, __FUNCTION__, __LINE__);
        fflush(stdout);
        return SC_CONNECTION_CLOSED;
    }
    else if(read_size == -1) {
        log_error("Receive failed", __FUNCTION__, __LINE__);
        return SC_RECEIVE_FAILED;
    }
    return SC_NO_ERROR;
}

ResponseStructure *get_player_response_by_number(SocketController *sc, unsigned number)
{
    switch (number) {
        case 1:
            return &sc->response_player1;
        case 2:
            return &sc->response_player2;
        default:
            return NULL;
    }
}

int sc_send_response(SocketController *sc, unsigned number_of_player)
{
    if (number_of_player != 1 && number_of_player != 2) {
        log_error("Unsupported player", __FUNCTION__, __LINE__);
        return SC_UNSUPPORTED_PLAYER;
    }
    int *player_socket = get_player_socket_by_number(sc, number_of_player);
    ResponseStructure *player_response = get_player_response_by_number(sc, number_of_player);
    if(send(*player_socket, player_response, sizeof(ResponseStructure), MSG_DONTWAIT) < 0)
    {
        log_error("Send failed", __FUNCTION__, __LINE__);
        return SC_SEND_FAILED;
    }
    return SC_NO_ERROR;
}

int sc_send_start_signal(SocketController *sc, unsigned number_of_player)
{
    int *player_socket = get_player_socket_by_number(sc, number_of_player);
    if (player_socket == NULL) {
        log_error("Unsupported player", __FUNCTION__, __LINE__);
        return SC_UNSUPPORTED_PLAYER;
    }
    char start_signal = SERVER_START_SIGNAL;
    if(send(*player_socket, &start_signal, SERVER_START_SIGNAL_LENGTH, MSG_CONFIRM) < 0)
    {
        log_error("Send start response failed", __FUNCTION__, __LINE__);
        return SC_SEND_FAILED;
    }
    return SC_NO_ERROR;
}