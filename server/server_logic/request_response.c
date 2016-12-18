//
// Created by moonlightnvkz on 19.11.16.
//
#include <stdio.h>
#include "request_response.h"
#include "../game_logic/player.h"
#include "../loggers.h"
#include "../game_logic/bullets.h"

void response_set_player_states(PlayerStateResponse *state, Player *player)
{
    state->angle = player->angle;
    state->is_alive = player->is_alive;
    state->position.x = player->geometry.x;
    state->position.y = player->geometry.y;
    state->velocity.x = player->velocity.x;
    state->velocity.y = player->velocity.y;
}

void response_set_bullets_states(BulletsStateResponse *state, Bullets *bullets)
{
    state->number = bullets->number;
    for (unsigned i = 0; i < bullets->number; ++i) {
        BulletStateResponse *bulletStateResponse = &state->bullets[i];
        Bullet *bullet = &bullets->bullets[i];
        bulletStateResponse->ttl = bullet->ttl;
        bulletStateResponse->angle = bullet->angle;
        bulletStateResponse->position.x = bullet->geometry.x;
        bulletStateResponse->position.y = bullet->geometry.y;
    }
}

void request_log(RequestStructure *req, char* msg, const char* function, const unsigned line)
{
    char buf[200] = {'\0'};
    sprintf(buf, "%s:%d|%lf, %f, %f, %d, %d, %d, %d",
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
    sprintf(buf, "%s:%d|%lf, %f, %f, %d, %d|%lf, %f, %f, %d, %d|%d",
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