//
// Created by moonlightnvkz on 19.11.16.
//
#include <stdlib.h>
#include <stdio.h>
#include "response_request.h"
#include "../game_logic/player.h"
#include "../loggers.h"

void response_set_player_states(PlayerStateResponse *state, Player *player)
{
    state->angle = player->angle;
    state->position.x = player->geometry.x;
    state->position.y = player->geometry.y;
    state->velocity.x = player->velocity.x;
    state->velocity.y = player->velocity.y;
}

void request_log(RequestStructure *req, char* msg, const char* function, const unsigned line)
{
    char buf[200] = {'\0'};
    sprintf(buf, "%s:%d|%lf, %f, %f, %d, %d.",
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