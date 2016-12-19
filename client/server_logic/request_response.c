//
// Created by moonlightnvkz on 19.11.16.
//
#include <stdlib.h>
#include <stdio.h>
#include "request_response.h"
#include "../game_logic/player.h"
#include "../loggers.h"
#include "../game_logic/mvc.h"

void request_set_player_states(RequestStructure *request, Player *player)
{
    request->player_state.velocity.x = player->velocity.x;
    request->player_state.velocity.y = player->velocity.y;
    request->player_state.angle = player->angle;
    request->player_state.position.x = player->geometry.x;
    request->player_state.position.y = player->geometry.y;
}

RequestStructure *request_create(MVC *mvc, unsigned number)
{
    RequestStructure *request = malloc(sizeof(RequestStructure));
    if (request == NULL) {
        log_error("Failed to allocate memory for request", __FUNCTION__, __LINE__);
        return NULL;
    }

    Player *player = &mvc->players[GlobalVariables.number_of_player];

    request->req_number = number;
    request_set_player_states(request, player);

    request->critical_event.type = mvc->criticalEvent.type;
    request->critical_event.description = mvc->criticalEvent.description;
    mvc->criticalEvent.type = CE_NONE;
    return request;
}

void request_destroy(RequestStructure *req)
{
    free(req);
}

void request_log(RequestStructure *req, char* msg, const char* function, const unsigned line)
{
    char buf[200] = {'\0'};
    sprintf(buf, "%s:%d|%lf, %f, %f, %d, %d|%d, %d",
            msg,
            req->req_number,
            req->player_state.angle,
            req->player_state.position.x,
            req->player_state.position.y,
            req->player_state.velocity.x,
            req->player_state.velocity.y,
            req->critical_event.type,
            req->critical_event.description);
    log_action(buf, function, line);
}

void response_log(ResponseStructure *res, char* msg, const char* function, const unsigned line)
{
    char buf[200] = {'\0'};
    sprintf(buf, "%s:%d|%lf, %f, %f, %d, %d|%lf, %f, %f, %d, %d|BULNUM:%d PL0:%d PL1:%d",
            msg,
            res->res_number,
            res->players[0].angle,
            res->players[0].position.x,
            res->players[0].position.y,
            res->players[0].velocity.x,
            res->players[0].velocity.y,
            res->players[1].angle,
            res->players[1].position.x,
            res->players[1].position.y,
            res->players[1].velocity.x,
            res->players[1].velocity.y,
            res->bullets.number,
            res->players[0].is_alive,
            res->players[1].is_alive);
    log_action(buf, function, line);
}