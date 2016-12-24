//
// Created by moonlightnvkz on 19.11.16.
//
#include <stdio.h>
#include "request_response.h"
#include "../loggers.h"
#include "../game_logic/player.h"
#include "../game_logic/bullets.h"
#include "../game_logic/explosives.h"

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

void response_set_explosives_state(ExplosivesStateResponse *state, Explosives *explosives)
{
    state->number = explosives->number;
    for (unsigned i = 0; i < explosives->number; ++i) {
        Explosive *explosive = &explosives->explosives[i];
        ExplosiveStateResponse *exp_state = &state->explosives[i];
        exp_state->is_damaged = explosive->is_damaged;
        exp_state->is_exploding = explosive->is_exploding;
        exp_state->timer_damaged = explosive->timer_damaged;
        exp_state->timer_explosion = explosive->timer_explosion;
        exp_state->position_at_map.x = explosive->position_at_map.x;
        exp_state->position_at_map.y = explosive->position_at_map.y;
    }
}

void request_log(RequestStructure *req, char* msg, const char* function, const unsigned line)
{
//    LOG_ACTION("%s:%d:%s|%d|%d %d",
//               function,
//               line,
//               msg,
//               req->req_number,
//               req->critical_event.type,
//               req->critical_event.description);
}

void response_log(ResponseStructure *res, char* msg, const char* function, const unsigned line)
{
//    LOG_ACTION("%s:%d:%s|%d %d %d %d %d",
//               function,
//               line,
//               msg,
//               res->res_number,
//               res->players[0].is_alive,
//               res->players[1].is_alive,
//               res->bullets.number,
//               res->explosives.number);
}