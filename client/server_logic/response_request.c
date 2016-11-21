//
// Created by moonlightnvkz on 19.11.16.
//
#include <stdlib.h>
#include <stdio.h>
#include "response_request.h"
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
    return req;
}

void request_destroy(RequestStructure *req)
{
    free(req);
}

bool request_compare(RequestStructure *req1, RequestStructure *req2)
{
    return req2->player_state.angle - req1->player_state.angle > 0.01
           && req1->player_state.position.x - req2->player_state.position.x > 0.01
           && req1->player_state.position.y - req2->player_state.position.y > 0.01
           && req1->player_state.velocity.x != req2->player_state.velocity.x
           && req1->player_state.velocity.y != req2->player_state.velocity.y;
}

void request_log(RequestStructure *req, char* msg, const char* function, const unsigned line)
{
    char buf[200] = {'\0'};
    sprintf(buf, "%s:%d|%lf, %f, %f, %d, %d",
            msg,
            req->req_number,
            req->player_state.angle,
            req->player_state.position.x,
            req->player_state.position.y,
            req->player_state.velocity.x,
            req->player_state.velocity.y);
    log_action(buf, function, line);
}

void response_log(ResponseStructure *res, char* msg, const char* function, const unsigned line)
{
    char buf[200] = {'\0'};
    sprintf(buf, "%s:%d|%lf, %f, %f, %d, %d|%lf, %f, %f, %d, %d",
            msg,
            res->res_number,
            res->this_player_state.angle,
            res->this_player_state.position.x,
            res->this_player_state.position.y,
            res->this_player_state.velocity.x,
            res->this_player_state.velocity.y,
            res->diff_player_state.angle,
            res->diff_player_state.position.x,
            res->diff_player_state.position.y,
            res->diff_player_state.velocity.x,
            res->diff_player_state.velocity.y);
    log_action(buf, function, line);
}