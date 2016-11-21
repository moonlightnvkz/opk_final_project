//
// Created by moonlightnvkz on 19.11.16.
//

#include <stdio.h>
#include <SDL_timer.h>
#include "socket_controller.h"
#include "../default_values.h"
#include "../game_logic/player.h"
#include "../loggers.h"

int sc_connect_to_server(SocketController *sc);

int sc_send_data_to_the_server(SocketController *sc, RequestStructure *request, size_t size);

int sc_receive_reply_from_the_server(SocketController *sc, ResponseStructure *reply, size_t size);

SocketController *sc_init()
{
    SocketController *sc = malloc(sizeof(SocketController));
    if (sc == NULL) {
        log_error("Failed to allocate memory for socket controller", __FUNCTION__, __LINE__);
        return NULL;
    }

    deque_create(&sc->requests_list);

    if (sc_connect_to_server(sc) != 0) {
        log_error("Failed to coonect to the server", __FUNCTION__, __LINE__);
        sc_destroy(sc);
        return NULL;
    }

    sc->last_response.res_number = 0;
    sc->last_response.this_player_state.position.x = sc->last_response.this_player_state.position.y = 0;
    sc->last_response.this_player_state.angle = 0;
    return sc;
}
void sc_destroy(SocketController *sc)
{
    if (shutdown(sc->sock, 2) == -1) {
        log_error("Failed to close the socket", __FUNCTION__, __LINE__);
    };
    deque_destroy(&sc->requests_list);
    free(sc);
}

int sc_connect_to_server(SocketController *sc) {
    //Create socket
    sc->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sc->sock == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    sc->server.sin_addr.s_addr = inet_addr(SERVER_IP);
    sc->server.sin_family = AF_INET;
    sc->server.sin_port = htons(SERVER_PORT);

    //Connect to remote server
    if (connect(sc->sock, (struct sockaddr *) &sc->server, sizeof(sc->server)) < 0) {
        perror("Connect failed. Error");
        return 1;
    }

    puts("Connected\n");
    return 0;
}

int sc_send_current_state(SocketController *sc, Player *player)
{
    static unsigned last_send_time = 0;
    if (last_send_time < 1 / SERVER_TICKRATE * 1000) {
        return 0;
    }
    RequestStructure *peeked = ((RequestStructure *) deque_peek_last(&sc->requests_list));
    unsigned last_number;
    if (peeked == NULL) {
        last_number = 0;
    } else {
        last_number = peeked->req_number;
    }
    RequestStructure *req = request_create(player, last_number + 1);
    deque_add_last(&sc->requests_list, (Pointer) req);

    if (sc_send_data_to_the_server(sc, req, sizeof(RequestStructure)) != 0) {
        log_error("An error occured while sending request", __FUNCTION__, __LINE__);
        return 1;
    }

    last_send_time = SDL_GetTicks();
    return 0;

}

// @return:
// 0 - all is Ok
// 1 - failed to receive
int sc_receive_current_state(SocketController *sc) {
    if (sc_receive_reply_from_the_server(sc, &sc->last_response, sizeof(ResponseStructure)) != 0) {
        return 1;
    }
    return 0;
}

int sc_send_data_to_the_server(SocketController *sc, RequestStructure *request, size_t size) {
    if (send(sc->sock, request, size, MSG_DONTWAIT) < 0) {
        log_error("Send failed", __FUNCTION__, __LINE__);
        return 1;
    }
    request_log(request, "Send", __FUNCTION__, __LINE__);
    return 0;
}

int sc_receive_reply_from_the_server(SocketController *sc, ResponseStructure *reply, size_t size) {
    if (recv(sc->sock, reply, size, MSG_DONTWAIT) < 0) {
        log_error("Receive failed", __FUNCTION__, __LINE__);
        return 1;
    }
    response_log(reply, "Recv", __FUNCTION__, __LINE__);
    return 0;
}