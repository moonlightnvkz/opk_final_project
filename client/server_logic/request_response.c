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
        LOG_ERROR("Failed to allocate memory for request");
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
    LOG_ACTION("%s:%d:%s|%d|%d %d",
               function,
               line,
               msg,
               req->req_number,
               req->critical_event.type,
               req->critical_event.description);
}

void response_log(ResponseStructure *res, char* msg, const char* function, const unsigned line)
{
    LOG_ACTION("%s:%d:%s|%d %d %d %d %d",
               function,
               line,
               msg,
               res->res_number,
               res->players[0].is_alive,
               res->players[1].is_alive,
               res->bullets.number,
               res->explosives.number);
}