//
// Created by moonlightnvkz on 19.11.16.
//

#include <stdio.h>
#include <unistd.h>    //close
#include <SDL_timer.h>
#include <assert.h>
#include <signal.h>
#include "socket_controller.h"
#include "../game_logic/player.h"
#include "../loggers.h"
#include "../game_logic/mvc.h"
#include "../globals.h"

static int sc_connect_to_server(SocketController *sc);

static void sc_clear_response(ResponseStructure *res);

static bool read_ip(const char *file, char *ip, uint16_t *port);

bool sc_init(SocketController *sc)
{
    assert(sc != NULL);

    deque_create(&sc->requests_list);

    if (sc_connect_to_server(sc) != 0) {
        sc_destroy(sc);
        return false;
    }

    sc_clear_response(&sc->last_response);

    signal(SIGPIPE, SIG_IGN);
    return true;
}

void sc_destroy(SocketController *sc)
{
    if (shutdown(sc->sock, 2) == -1) {
        LOG_ERROR("Failed to close the socket");
    };
    close(sc->sock);
    deque_destroy(&sc->requests_list);
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
    for (unsigned i = 0; i < EXPLOSIVE_MAX_AMOUNT; ++i) {
        ExplosiveStateResponse *e = &res->explosives.explosives[i];
        e->is_damaged = e->is_exploding = false;
        e->timer_damaged = e->timer_explosion = 0;
        e->position_at_map.x = e->position_at_map.y = 0;
    }
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        PlayerStateResponse *p = &res->players[i];
        p->angle = 0;
        p->is_alive = true;
        p->position.x = PLAYER_START_X;
        p->position.y = PLAYER_START_Y;
        p->velocity.x = p->velocity.y = 0;
    }
}

static int sc_connect_to_server(SocketController *sc) {
    //Create socket
    sc->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sc->sock == -1) {
        LOG_ERROR("Failed to create socket");
        return SC_SOCKET_CREATION_ERROR;
    }

    // "xxx.xxx.xxx.xxx\0"
    char ip[16] = {0};
    uint16_t port = 0;
    if (!read_ip(SERVER_CONF_FILE, ip, &port)) {
        return SC_READ_IP_FAILED;
    }

    sc->server.sin_addr.s_addr = inet_addr(ip);
    sc->server.sin_family = AF_INET;
    sc->server.sin_port = htons(port);

    //Connect to remote server
    if (connect(sc->sock, (struct sockaddr *) &sc->server, sizeof(sc->server)) < 0) {
        LOG_ERROR("Failed to connect to the server");
        return SC_CONNECTION_FAILED;
    }

    LOG_ACTION("Connected");
    return SC_NO_ERROR;
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

int sc_send_current_state(SocketController *sc, MVC *mvc)
{
    static unsigned last_send_time = 0;
    if (mvc->critical_event.type == CE_NONE && (SDL_GetTicks() - last_send_time) < 1000.0 / SERVER_TICKRATE) {
        return SC_NO_ERROR;
    }
    RequestStructure *peeked = ((RequestStructure *) deque_peek_last(&sc->requests_list));
    unsigned last_number;
    if (peeked == NULL) {
        last_number = 0;
    } else {
        last_number = peeked->req_number;
    }
    RequestStructure *req = request_create(mvc, last_number + 1);
    deque_add_last(&sc->requests_list, (Pointer) req);

    last_send_time = SDL_GetTicks();
    if (send(sc->sock, req, sizeof(RequestStructure), MSG_DONTWAIT | MSG_NOSIGNAL) < 0) {
        LOG_ERROR("Send failed");
        return SC_SEND_FAILED;
    }
    request_log(req, "Send", __FUNCTION__, __LINE__);

    mvc->critical_event.type = CE_NONE;

    return SC_NO_ERROR;
}

// @return:
// 0 - all is Ok
// 1 - failed to receive
int sc_receive_current_state(SocketController *sc) {
    static ssize_t left_to_receive = sizeof(ResponseStructure);
    ssize_t res = recv(sc->sock, &sc->last_response, sizeof(ResponseStructure), MSG_DONTWAIT);
    if (res == -1) {
        //LOG_ERROR("Receive failed");
        return SC_RECEIVE_FAILED;
    }
    if (res == 0) {
        LOG_ERROR("Connection closed");
        return SC_CONNECTION_CLOSED;
    }
    left_to_receive -= res;

    if (left_to_receive < 0) {
        LOG_ERROR("Left to receive is bellow zero");
        return SC_RECEIVE_FAILED;
    }

    if (left_to_receive > 0) {
        return SC_PARTIAL_RECEIVE;
    }
    left_to_receive = sizeof(ResponseStructure);
    response_log(&sc->last_response, "Recv", __FUNCTION__, __LINE__);
    return SC_NO_ERROR;
}

int sc_receive_start_signal(SocketController *sc)
{
    StartSignal start_signal = {0, 0};
    if (recv(sc->sock, &start_signal, sizeof(StartSignal), MSG_WAITALL) <= 0) {
        LOG_ERROR("Receive of start signal failed");
        return SC_RECEIVE_FAILED;
    }
    if (start_signal.symbol != SERVER_START_SIGNAL) {
        return SC_START_SIGNAL_MATHCHING_ERROR;
    }
    GlobalVariables.number_of_the_player = start_signal.number_of_player;
    LOG_ERROR("Start signal received:%c - %d", start_signal.symbol, GlobalVariables.number_of_the_player);
    return SC_NO_ERROR;
}
