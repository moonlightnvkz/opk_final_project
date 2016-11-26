//
// Created by moonlightnvkz on 10.11.16.
//

#include <math.h>
#include <SDL_mouse.h>
#include <SDL_timer.h>
#include "player.h"
#include "bullet.h"
#include "../default_values.h"
#include "../loggers.h"
#include "../server_logic/request_response.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846	/* pi */
#endif

static void player_move_on(Player *player, float x, float y);

static void player_move_to(Player *player, float x, float y);

Player *player_create()
{
    Player *player = malloc(sizeof(Player));
    if (player == NULL) {
        log_error("Failed to allocate memory for player", __FUNCTION__, __LINE__);
        exit(1);
    }
    player->geometry.x = PLAYER_X;
    player->geometry.y = PLAYER_Y;
    player->geometry.width = PLAYER_WIDTH;
    player->geometry.height = PLAYER_HEIGHT;
    player->angle = 0.0;
    player->velocity.x = 0;
    player->velocity.y = 0;
    player->last_request_time = 0;
    player->last_shot_time = 0;
    return player;
}

void player_destroy(Player *player)
{
    free(player);
}

void player_move(Player *player, unsigned delta_ticks)
{
    float dx = (float) delta_ticks / 1000 * player->velocity.x;
    float dy = (float) delta_ticks / 1000 * player->velocity.y;
    player_move_on(player, dx, dy);
}

static void player_move_to(Player *player, float x, float y)
{
    player->geometry.x = x;
    player->geometry.y = y;
}

static void player_move_on(Player *player, float x, float y)
{
    player->geometry.x += x;
    player->geometry.y += y;
}

void player_do_shot(Player *player, Bullets *bullets)
{
    unsigned time = SDL_GetTicks();
    if (time - player->last_shot_time < (float)1 / PLAYER_FIRE_RATE * 1000) {
        return;
    }
    if (bullets->number == BULLET_MAX_AMOUNT) {
        return;
    }
    bullets->bullets[bullets->number].angle = player->angle;
    bullets->bullets[bullets->number].geometry.x = player->geometry.x + player->geometry.width / 2;
    bullets->bullets[bullets->number].geometry.y = player->geometry.y + player->geometry.height / 2;
    bullets->bullets[bullets->number].active = true;
    bullets->number++;
    player->last_shot_time = time;
}

void player_apply_request(Player *player, RequestStructure *request)
{
    player->angle = request->player_state.angle;
    player->geometry.x = request->player_state.position.x;
    player->geometry.y = request->player_state.position.y;
    player->velocity.x = request->player_state.velocity.x;
    player->velocity.y = request->player_state.velocity.y;
    player->last_request_time = SDL_GetTicks();
    player->shot_done = request->player_state.shot_done;
}