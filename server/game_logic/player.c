//
// Created by moonlightnvkz on 10.11.16.
//

#include <math.h>
#include <SDL_mouse.h>
#include <SDL_timer.h>
#include <assert.h>
#include "../default_values.h"
#include "player.h"
#include "bullets.h"
#include "../server_logic/request_response.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846	/* pi */
#endif

static void player_move_on(Player *player, float dx, float dy);

bool player_create(Player * player)
{
    assert(player != NULL);
    player->is_alive = true;
    player->geometry.x = PLAYER_X;
    player->geometry.y = PLAYER_Y;
    player->geometry.width = PLAYER_WIDTH;
    player->geometry.height = PLAYER_HEIGHT;
    player->angle = 0;
    player->velocity.x = 0;
    player->velocity.y = 0;
    player->last_request_time = 0;
    player->last_shot_time = 0;
    return true;
}

void player_destroy(Player *player)
{
    return;
}

void player_move(Player *player, unsigned delta_ticks)
{
    float dx = (float) delta_ticks / 1000 * player->velocity.x;
    float dy = (float) delta_ticks / 1000 * player->velocity.y;
    player_move_on(player, dx, dy);
}

static void player_move_on(Player *player, float dx, float dy)
{
    ObjectGeometry new_geom = {player->geometry.x + dx,
                               player->geometry.y,
                               player->geometry.width,
                               player->geometry.height};
    if (geometry_rect_rect_collision_check(new_geom, true, GlobalVariables.map_geometry)) {
        player->geometry.x += dx;
    }
    new_geom.x -= dx;
    new_geom.y += dy;
    if (geometry_rect_rect_collision_check(new_geom, true, GlobalVariables.map_geometry)) {
        player->geometry.y += dy;
    }
}

void player_do_shot(Player *player, Bullets *bullets)
{
    unsigned time = SDL_GetTicks();
    if (time - player->last_shot_time < (float)1 / PLAYER_FIRE_RATE * 1000) {
        return;
    }
    Vector2f bullet_pos = {player->geometry.x +
                           player->geometry.width * (float) sin(deg_to_rad(player->angle)) / 2.f,
                           player->geometry.y +
                           player->geometry.width * (float) cos(deg_to_rad(player->angle)) / 2.f};
    if (bullets_add_bullet(bullets, bullet_pos, player->angle)) {
        player->last_shot_time = time;
    }
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