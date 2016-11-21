//
// Created by moonlightnvkz on 20.11.16.
//

#include <SDL_timer.h>
#include "model_controller.h"
#include "player.h"
#include "bullet.h"
#include "../server_logic/response_request.h"
#include "../loggers.h"
#include "../default_values.h"

void mc_check_request_and_fix(Player *player, RequestStructure *request);

ModelController *mc_init()
{
    ModelController *mc = malloc(sizeof(ModelController));
    if (mc == NULL) {
        log_error("Failed to allocate memory for model controller", __FUNCTION__, __LINE__);
        return NULL;
    }
    mc->player1 = player_create();
    if (mc->player1 == NULL) {
        log_error("Failed to allocate memory for player1", __FUNCTION__, __LINE__);
        mc_destroy(mc);
        return NULL;
    }
    mc->player2 = player_create();
    if (mc->player2 == NULL) {
        log_error("Failed to allocate memory for player2", __FUNCTION__, __LINE__);
        mc_destroy(mc);
        return NULL;
    }

    mc->bullets = bullet_create();
    if (mc->bullets == NULL) {
        log_error("Failed to allocate memory for bullets", __FUNCTION__, __LINE__);
        mc_destroy(mc);
        return NULL;
    }
    return mc;
}

void mc_destroy(ModelController *mc)
{
    if (mc->player1 != NULL) {
        player_destroy(mc->player1);
    }
    if (mc->player2 != NULL) {
        player_destroy(mc->player2);
    }
    if (mc->bullets != NULL) {
        bullet_destroy(mc->bullets);
    }
    free(mc);
}

void mc_process_moving(ModelController *mc, unsigned delta_ticks)
{
    player_move(mc->player1, delta_ticks);
    player_move(mc->player2, delta_ticks);
    bullet_move_all(mc->bullets, delta_ticks);
}

void mc_apply_request(ModelController *mc, Player *player, RequestStructure *request)
{
    mc_check_request_and_fix(player, request);
    player_apply_request(player, request);
}

void mc_check_request_and_fix(Player *player, RequestStructure *request) {
    unsigned time = SDL_GetTicks();
    if (request->player_state.velocity.x > PLAYER_VELOCITY) {
        request->player_state.velocity.x = PLAYER_VELOCITY;
    }
    if (request->player_state.velocity.y > PLAYER_VELOCITY) {
        request->player_state.velocity.y = PLAYER_VELOCITY;
    }
    unsigned time_elapsed = time - player->last_request_time;
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
}

void mc_create_response(ModelController *mc, int player, unsigned req_number, ResponseStructure *res)
{
    res->res_number = req_number;
    switch (player) {
        case 1:
            response_set_player_states(&res->this_player_state, mc->player1);
            response_set_player_states(&res->diff_player_state, mc->player2);
            break;
        case 2:
            response_set_player_states(&res->this_player_state, mc->player2);
            response_set_player_states(&res->diff_player_state, mc->player1);
        default:
            log_error("Unsupported player", __FUNCTION__, __LINE__);
    }
}