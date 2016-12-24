//
// Created by moonlightnvkz on 20.11.16.
//

#include <SDL_timer.h>
#include <assert.h>
#include "model_controller.h"
#include "player.h"
#include "bullets.h"
#include "../server_logic/request_response.h"
#include "../loggers.h"
#include "../default_values.h"
#include "explosives.h"

#ifndef M_SQRT2
#define M_SQRT2	1.41421356237309504880	/* sqrt(2) */
#endif

void mc_check_request_and_fix(ModelController *mc, unsigned number_of_player, RequestStructure *request);

bool mc_init(ModelController *mc)
{
    assert(mc != NULL);
    for (size_t i = 0; i < PLAYER_COUNT; ++i) {
        if (!player_create(mc->players + i)) {
            LOG_ERROR("Failed to create player");
            mc_destroy(mc);
            return false;
        }
    }

    if (!bullets_create(&mc->bullets)) {
        LOG_ERROR("Failed to cerate bullets");
        mc_destroy(mc);
        return false;
    }

    if (!tilemap_create(&mc->map)) {
        LOG_ERROR("Failed to create tile_map");
        mc_destroy(mc);
        return false;
    }
    return true;
}

void mc_destroy(ModelController *mc)
{
    for (size_t i = 0; i < PLAYER_COUNT; ++i) {
        player_destroy(mc->players + i);
    }
    bullets_destroy(&mc->bullets);
    tilemap_destroy(&mc->map);
}

void mc_process_moving(ModelController *mc, unsigned delta_ticks)
{
    for (size_t i = 0; i < PLAYER_COUNT; ++i) {
        player_move(mc->players + i, delta_ticks);
    }
    bullets_move_all(&mc->bullets, delta_ticks, mc->players);

    explosives_explode_process(&mc->map.explosives, delta_ticks, mc->players);
}

int mc_alive_players_count(ModelController *mc)
{
    unsigned count = 0;
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        if (mc->players[i].is_alive) {
            count++;
        }
    }
    return count;
}

void mc_apply_request(ModelController *mc, unsigned number_of_player, RequestStructure *request)
{
    mc_check_request_and_fix(mc, number_of_player, request);

    player_apply_request(&mc->players[number_of_player], request);
    if (request->critical_event.type == CE_SHOT_DONE && request->critical_event.description == number_of_player) {
        player_do_shot(&mc->players[number_of_player], &mc->bullets);
    }
    if (request->critical_event.type == CE_DEATH) {
        player_kill(&mc->players[request->critical_event.description]);
    }
}

void mc_check_request_and_fix(ModelController *mc, unsigned number_of_player, RequestStructure *request)
{
    Player *player = &mc->players[number_of_player];
    if (request->player_state.velocity.x > PLAYER_VELOCITY) {
        request->player_state.velocity.x = PLAYER_VELOCITY;
    }
    if (request->player_state.velocity.y > PLAYER_VELOCITY) {
        request->player_state.velocity.y = PLAYER_VELOCITY;
    }
//    if (request->player_state.velocity.x != 0 && request->player_state.velocity.y != 0) {
//        player->velocity.x /= M_SQRT2;
//        player->velocity.y /= M_SQRT2;
//    }
    unsigned time_elapsed = SDL_GetTicks() - player->last_request_time;
    float shift_x = request->player_state.position.x - player->geometry.x;
    float shift_y = request->player_state.position.y - player->geometry.y;
    if (shift_x / time_elapsed * 1000 > PLAYER_VELOCITY) {
        request->player_state.position.x = request->player_state.position.x +
                                           (float) time_elapsed / 1000 * PLAYER_VELOCITY;
    }
    if (shift_y / time_elapsed * 1000 > PLAYER_VELOCITY) {
        request->player_state.position.y = request->player_state.position.y +
                                           (float) time_elapsed / 1000 * PLAYER_VELOCITY;
    }
    if (request->critical_event.type == CE_SHOT_DONE && (SDL_GetTicks() - player->last_shot_time < (float) 1000 / PLAYER_FIRE_RATE)) {
        LOG_ERROR("Too high firerate");
        request->critical_event.type = CE_NONE;
    }
}