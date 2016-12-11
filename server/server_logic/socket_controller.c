//
// Created by moonlightnvkz on 20.11.16.
//

#include <malloc.h>
#include <sys/socket.h>
#include <unistd.h>    //write
#include "../default_values.h"
#include "socket_controller.h"
#include "../loggers.h"
#include "../game_logic/model_controller.h"

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
    sc->server.sin_addr.s_addr = inet_addr(SERVER_IP);
    sc->server.sin_port = htons(SERVER_PORT);

    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        sc->request_numbers[i] = 0;
    }
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
    for (size_t i = 0; i < PLAYER_COUNT; ++i) {
        shutdown(sc->player_sockets[i], 2);
        close(sc->player_sockets[i]);
    }
    close(sc->socket_desc);
    free(sc);
}

int sc_accept_player(SocketController *sc, unsigned number_of_player)
{

    if (number_of_player >= PLAYER_COUNT) {
        log_error("Unsupported player", __FUNCTION__, __LINE__);
        return SC_UNSUPPORTED_PLAYER;
    }
    char msg[50];
    sprintf(msg, "Waiting for incoming connection: player %d", number_of_player);
    log_action(msg, __FUNCTION__, __LINE__);

    sc->player_sockets[number_of_player] = accept(sc->socket_desc, NULL, NULL);
    if (sc->player_sockets[number_of_player] < 0) {
        log_error("Failed to accept player connection", __FUNCTION__, __LINE__);
        return SC_CONNECTION_FAILED;
    }
    sprintf(msg, "Connection accepted:player %d", number_of_player);
    log_action(msg, __FUNCTION__, __LINE__);
    return SC_NO_ERROR;
}

int sc_receive_request(SocketController *sc, unsigned number_of_player)
{
    if (number_of_player >= PLAYER_COUNT) {
        log_error("Unsupported player", __FUNCTION__, __LINE__);
        return SC_UNSUPPORTED_PLAYER;
    }

    ssize_t read_size = recv(sc->player_sockets[number_of_player] , &sc->request , sizeof(RequestStructure) , 0);
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
    sc->request_numbers[number_of_player] = sc->request.req_number;
    return SC_NO_ERROR;
}

void sc_create_responses(SocketController *sc, ModelController *mc)
{
    for (size_t i = 0; i < PLAYER_COUNT; ++i) {
        response_set_player_states(&sc->response.players[i], mc->players[i]);
        response_set_bullets_states(&sc->response.bullets, mc->bullets);
    }
}

int sc_send_response(SocketController *sc, unsigned number_of_player)
{
    if (number_of_player >= PLAYER_COUNT) {
        log_error("Unsupported player", __FUNCTION__, __LINE__);
        return SC_UNSUPPORTED_PLAYER;
    }
    sc->response.res_number = sc->request_numbers[number_of_player];
    if(send(sc->player_sockets[number_of_player], &sc->response, sizeof(ResponseStructure), MSG_DONTWAIT) < sizeof(ResponseStructure))
    {
        puts("1");
        char msg[50];
        sprintf(msg, "Send failed (player %d)", number_of_player);
        log_error(msg, __FUNCTION__, __LINE__);
        return SC_SEND_FAILED;
    }
    return SC_NO_ERROR;
}

int sc_send_start_signal(SocketController *sc, unsigned number_of_player)
{
    if (number_of_player >= PLAYER_COUNT) {
        log_error("Unsupported player", __FUNCTION__, __LINE__);
        return SC_UNSUPPORTED_PLAYER;
    }
    StartSignal start_signal = {SERVER_START_SIGNAL, number_of_player};
    if(send(sc->player_sockets[number_of_player], &start_signal, sizeof(StartSignal), MSG_CONFIRM) < 0)
    {
        log_error("Send start response failed", __FUNCTION__, __LINE__);
        return SC_SEND_FAILED;
    }
    return SC_NO_ERROR;
}