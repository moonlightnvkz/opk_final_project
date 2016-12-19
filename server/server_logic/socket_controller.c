//
// Created by moonlightnvkz on 20.11.16.
//

#include <malloc.h>
#include <sys/socket.h>
#include <unistd.h>    //write
#include <assert.h>
#include "../default_values.h"
#include "socket_controller.h"
#include "../loggers.h"
#include "../game_logic/model_controller.h"

bool sc_init(SocketController *sc)
{
    assert(sc != NULL);

    sc->socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (sc->socket_desc == -1) {
        LOG_ERROR("Failed to create socket");
        return false;
    }
    sc->server.sin_family = AF_INET;
    sc->server.sin_addr.s_addr = inet_addr(SERVER_IP);
    sc->server.sin_port = htons(SERVER_PORT);

    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        sc->request_numbers[i] = 0;
    }
    if (bind(sc->socket_desc, (struct sockaddr *) &sc->server, sizeof(sc->server)) < 0) {
        LOG_ERROR("Failed to bind");
        return false;
    }

    listen(sc->socket_desc , 3);

    LOG_ERROR("Server listen at %d:%d", INADDR_ANY, SERVER_PORT);
    return true;
}

void sc_destroy(SocketController *sc)
{
    for (size_t i = 0; i < PLAYER_COUNT; ++i) {
        shutdown(sc->player_sockets[i], 2);
        close(sc->player_sockets[i]);
    }
    close(sc->socket_desc);
}

int sc_accept_player(SocketController *sc, unsigned number_of_player)
{

    if (number_of_player >= PLAYER_COUNT) {
        LOG_ERROR("Unsupported player");
        return SC_UNSUPPORTED_PLAYER;
    }
    LOG_ACTION("Waiting for incoming connection: player %d", number_of_player);

    sc->player_sockets[number_of_player] = accept(sc->socket_desc, NULL, NULL);
    if (sc->player_sockets[number_of_player] < 0) {
        LOG_ERROR("Failed to accept player connection");
        return SC_CONNECTION_FAILED;
    }
    LOG_ACTION("Connection accepted:player %d", number_of_player);
    return SC_NO_ERROR;
}

int sc_receive_request(SocketController *sc, unsigned number_of_player)
{
    if (number_of_player >= PLAYER_COUNT) {
        LOG_ERROR("Unsupported player");
        return SC_UNSUPPORTED_PLAYER;
    }

    ssize_t read_size = recv(sc->player_sockets[number_of_player] , &sc->request , sizeof(RequestStructure) , 0);
    if(read_size == 0) {
        LOG_ACTION("Player %d disconnected", number_of_player);
        return SC_CONNECTION_CLOSED;
    }
    else if(read_size == -1) {
        LOG_ERROR("Receive failed");
        return SC_RECEIVE_FAILED;
    }
    sc->request_numbers[number_of_player] = sc->request.req_number;
    return SC_NO_ERROR;
}

void sc_create_responses(SocketController *sc, ModelController *mc, bool quit)
{
    for (size_t i = 0; i < PLAYER_COUNT; ++i) {
        response_set_player_states(&sc->response.players[i], mc->players + i);
    }
    response_set_bullets_states(&sc->response.bullets, &mc->bullets);
    response_set_explosives_state(&sc->response.explosives, &mc->map.explosives);

    sc->response.quit = quit;
}

int sc_send_response(SocketController *sc, unsigned number_of_player)
{
    if (number_of_player >= PLAYER_COUNT) {
        LOG_ERROR("Unsupported player");
        return SC_UNSUPPORTED_PLAYER;
    }
    sc->response.res_number = sc->request_numbers[number_of_player];
    if(send(sc->player_sockets[number_of_player], &sc->response, sizeof(ResponseStructure), MSG_DONTWAIT) < sizeof(ResponseStructure))
    {
        LOG_ERROR("Send failed (player %d)", number_of_player);
        return SC_SEND_FAILED;
    }
    return SC_NO_ERROR;
}

int sc_send_start_signal(SocketController *sc, unsigned number_of_player)
{
    if (number_of_player >= PLAYER_COUNT) {
        LOG_ERROR("Unsupported player");
        return SC_UNSUPPORTED_PLAYER;
    }
    StartSignal start_signal = {SERVER_START_SIGNAL, number_of_player};
    if(send(sc->player_sockets[number_of_player], &start_signal, sizeof(StartSignal), MSG_CONFIRM) < 0)
    {
        LOG_ERROR("Send start response failed");
        return SC_SEND_FAILED;
    }
    return SC_NO_ERROR;
}