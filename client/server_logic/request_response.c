//
// Created by moonlightnvkz on 19.11.16.
//
#include <stdlib.h>
#include <stdio.h>
#include "request_response.h"
#include "../game_logic/player.h"
#include "../loggers.h"
#include "../game_logic/mvc.h"

RequestStructure *request_create(MVC *mvc, unsigned number)
{
    RequestStructure *req = malloc(sizeof(RequestStructure));
    if (req == NULL) {
        log_error("Failed to allocate memory for request", __FUNCTION__, __LINE__);
        return NULL;
    }

    Player *player = &mvc->players[GlobalVariables.number_of_player];

    req->req_number = number;
    req->player_state.velocity.x = player->velocity.x;
    req->player_state.velocity.y = player->velocity.y;
    req->player_state.angle = player->angle;
    req->player_state.position.x = player->geometry.x;
    req->player_state.position.y = player->geometry.y;

    req->critical_event.type = mvc->criticalEvent.type;
    req->critical_event.number_of_player = mvc->criticalEvent.number_of_player;
    mvc->criticalEvent.type = CE_NONE;
    return req;
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
            req->critical_event.number_of_player);
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