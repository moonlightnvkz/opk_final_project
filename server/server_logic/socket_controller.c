//
// Created by moonlightnvkz on 20.11.16.
//

#include <malloc.h>
#include <sys/socket.h>
#include <unistd.h>    //write
#include <assert.h>
#include "../defines.h"
#include "socket_controller.h"
#include "../loggers.h"
#include "../game_logic/model_controller.h"
#include <signal.h>

static void sc_clear_response(ResponseStructure *res);

static bool read_ip(const char *file, char *ip, uint16_t *port);

bool sc_init(SocketController *sc)
{
    assert(sc != NULL);

    sc->socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (sc->socket_desc == -1) {
        LOG_ERROR("Failed to create socket");
        return false;
    }

    // "xxx.xxx.xxx.xxx\0"
    char ip[16] = {0};
    uint16_t port = 0;
    if (!read_ip(SERVER_CONF_FILE, ip, &port)) {
        return false;
    }
    sc->server.sin_family = AF_INET;
    sc->server.sin_addr.s_addr = inet_addr(ip);
    sc->server.sin_port = htons(port);

    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        sc->request_numbers[i] = 0;
    }
    if (bind(sc->socket_desc, (struct sockaddr *) &sc->server, sizeof(sc->server)) < 0) {
        LOG_ERROR("Failed to bind");
        return false;
    }

    sc_clear_response(&sc->response);
    listen(sc->socket_desc , 3);

    signal(SIGPIPE, SIG_IGN);

    LOG_ERROR("Server listen at %s:%d", ip, port);
    return true;
}

static bool read_ip(const char *file, char *ip, uint16_t *port)
{
    FILE *conf = fopen(file, "r");
    if (!conf) {
        LOG_ERROR("Failed to open server conf file");
        return false;
    }
    int ip_part = 0;
    int offset = 0;

    bool error = false;

    for (unsigned i = 0; i < 4; ++i) {
        if (fscanf(conf, "%d%*c", &ip_part) != 1) {
            LOG_ERROR("Wrong ip format");
            error = true;
            break;
        }
        if (ip_part > 255 || ip_part < 0) {
            LOG_ERROR("Wrong ip format:%d octet", i);
            error = true;
            break;
        }
        sprintf(ip + offset, "%d", ip_part);
        if (ip_part < 10) {
            offset += 1;
        } else if (ip_part < 100) {
            offset += 2;
        } else {
            offset +=3;
        }
        if (i != 3) {
            sprintf(ip + offset, ".");
            offset++;
        }
    }

    if (!error && fscanf(conf, "%hu", port) != 1) {
        LOG_ERROR("Wrong port format");
        error = true;
    }
    if (!error && port < 0) {
        LOG_ERROR("Port can't be negative");
        error = true;
    }
    fclose(conf);
    return !error;
}

void sc_destroy(SocketController *sc)
{
    for (size_t i = 0; i < PLAYER_COUNT; ++i) {
        shutdown(sc->player_sockets[i], 2);
        close(sc->player_sockets[i]);
    }
    close(sc->socket_desc);
}

static void sc_clear_response(ResponseStructure *res)
{
    res->res_number = 0;
    res->quit = false;
    res->bullets.number = 0;
    for (unsigned i = 0; i < BULLET_MAX_AMOUNT; ++i) {
        BulletStateResponse *b = &res->bullets.bullets[i];
        b->angle = 0;
        b->position.x = b->position.y = 0;
        b->ttl = 0;
    }
    res->explosives.number = 0;
    for (unsigned i = 0; i < BULLET_MAX_AMOUNT; ++i) {
        ExplosiveStateResponse *e = &res->explosives.explosives[i];
        e->is_damaged = e->is_exploding = false;
        e->timer_damaged = e->timer_explosion = 0;
        e->position_at_map.x = e->position_at_map.y = 0;
    }
    for (unsigned i = 0; i < BULLET_MAX_AMOUNT; ++i) {
        PlayerStateResponse *p = &res->players[i];
        p->angle = 0;
        p->is_alive = true;
        p->position.x = PLAYER_X;
        p->position.y = PLAYER_Y;
        p->velocity.x = p->velocity.y = 0;
    }
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

    ssize_t read_size = recv(sc->player_sockets[number_of_player] , &sc->request , sizeof(RequestStructure) , MSG_DONTWAIT);
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
    if(send(sc->player_sockets[number_of_player], &sc->response, sizeof(ResponseStructure), MSG_DONTWAIT | MSG_NOSIGNAL) < sizeof(ResponseStructure))
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