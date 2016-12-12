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

void mc_check_request_and_fix(Player *player, RequestStructure *request);

bool mc_init(ModelController *mc)
{
    assert(mc != NULL);
    for (size_t i = 0; i < PLAYER_COUNT; ++i) {
        if (!player_create(mc->players + i)) {
            log_error("Failed to allocate memory for player1", __FUNCTION__, __LINE__);
            mc_destroy(mc);
            return false;
        }
    }

    if (!bullets_create(&mc->bullets)) {
        log_error("Failed to allocate memory for bullets", __FUNCTION__, __LINE__);
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
}

void mc_process_moving(ModelController *mc, unsigned delta_ticks)
{
    for (size_t i = 0; i < PLAYER_COUNT; ++i) {
        player_move(mc->players + i, delta_ticks);
    }
    bullets_move_all(&mc->bullets, delta_ticks, mc->players);
}

void mc_apply_request(ModelController *mc, Player *player, RequestStructure *request)
{
    mc_check_request_and_fix(player, request);
    player_apply_request(player, request);
    if (request->player_state.shot_done) {
        player_do_shot(player, &mc->bullets);
    }
}

void mc_check_request_and_fix(Player *player, RequestStructure *request) {
    if (request->player_state.velocity.x > PLAYER_VELOCITY) {
        request->player_state.velocity.x = PLAYER_VELOCITY;
    }
    if (request->player_state.velocity.y > PLAYER_VELOCITY) {
        request->player_state.velocity.y = PLAYER_VELOCITY;
    }
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
    if (request->player_state.shot_done && (SDL_GetTicks() - player->last_shot_time < (float) 1000 / PLAYER_FIRE_RATE)) {
        log_error("Too high firerate", __FUNCTION__, __LINE__);
        request->player_state.shot_done = false;
    }
}

void mc_reset_temp_flags(ModelController *mc)
{
    for (size_t i = 0; i < PLAYER_COUNT; ++i) {
        mc->players[i].shot_done = false;
    }
}