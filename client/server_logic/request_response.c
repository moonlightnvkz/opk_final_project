//
// Created by moonlightnvkz on 19.11.16.
//
#include <stdlib.h>
#include <stdio.h>
#include "request_response.h"
#include "../game_logic/player.h"
#include "../loggers.h"

RequestStructure *request_create(Player *player, unsigned number)
{
    RequestStructure *req = malloc(sizeof(RequestStructure));
    if (req == NULL) {
        log_error("Failed to allocate memory for request", __FUNCTION__, __LINE__);
        return NULL;
    }
    req->req_number = number;
    req->player_state.velocity.x = player->velocity.x;
    req->player_state.velocity.y = player->velocity.y;
    req->player_state.angle = player->angle;
    req->player_state.position.x = player->geometry.x;
    req->player_state.position.y = player->geometry.y;
    req->player_state.shot_done = player->shot_done;
    player->shot_done = false;
    return req;
}

void request_destroy(RequestStructure *req)
{
    free(req);
}

void request_log(RequestStructure *req, char* msg, const char* function, const unsigned line)
{
    char buf[200] = {'\0'};
    sprintf(buf, "%s:%d|%d, %f, %f, %d, %d, %d",
            msg,
            req->req_number,
            req->player_state.angle,
            req->player_state.position.x,
            req->player_state.position.y,
            req->player_state.velocity.x,
            req->player_state.velocity.y,
            req->player_state.shot_done);
    log_action(buf, function, line);
}

void response_log(ResponseStructure *res, char* msg, const char* function, const unsigned line)
{
    char buf[200] = {'\0'};
    sprintf(buf, "%s:%d|%d, %f, %f, %d, %d|%d, %f, %f, %d, %d|%d",
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
            res->bullets.number);
    log_action(buf, function, line);
}