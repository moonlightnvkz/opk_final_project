//
// Created by moonlightnvkz on 10.11.16.
//

#include <assert.h>
#include <math.h>
#include <SDL_mouse.h>
#include <SDL_timer.h>
#include "../loggers.h"
#include "sdl_helpers.h"
#include "player.h"
#include "bullets.h"
#include "../server_logic/request_response.h"
#include "../my_deque.h"
#include "camera.h"
#include "tile_map.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846	/* pi */
#endif

static void player_move_on(Player *player, float dx, float dy);

static void player_move_to(Player *player, float x, float y);

bool player_create(Player *player, SDL_Renderer *renderer)
{
    assert(player != NULL);

    GlobalVariables.number_of_player = 0;
    player->is_alive = true;
    player->geometry.x = PLAYER_START_X;
    player->geometry.y = PLAYER_START_Y;
    player->geometry.width = PLAYER_WIDTH;
    player->geometry.height = PLAYER_HEIGHT;
    player->angle = 0;
    player->velocity.x = 0;
    player->velocity.y = 0;
    player->last_shot_time = 0;
    player->texture = load_texture(PLAYER_TEXTURE, renderer);
    if (player->texture == NULL) {
        LOG_ERROR("Failed to load texture");
        player->texture = load_texture(MISSING_TEXTURE, renderer);
        if (player->texture == NULL) {
            LOG_ERROR("Failed to load <missing_texture>");
            return NULL;
        }
    }

    return true;
}

void player_destroy(Player *player)
{
    SDL_DestroyTexture(player->texture);
}

void player_move(Player *player, unsigned delta_ticks)
{
    if (!player->is_alive) {
        return;
    }
    float dx = (float) delta_ticks / 1000 * player->velocity.x;
    float dy = (float) delta_ticks / 1000 * player->velocity.y;
    player_move_on(player, dx, dy);
}

static void player_move_to(Player *player, float x, float y)
{
    player->geometry.x = x;
    player->geometry.y = y;
}

static bool player_collision_check(Player *player, float dx, float dy)
{
    ObjectGeometry new_geom = {player->geometry.x + dx,
                               player->geometry.y + dy,
                               player->geometry.width,
                               player->geometry.height};
    if (!geometry_rect_rect_collision_check(new_geom, true, GlobalVariables.map_geometry)) {
        return false;
    }
    if (!tilemap_collision_check(new_geom)) {
        return false;
    }
    return true;
}

static void player_move_on(Player *player, float dx, float dy)
{
    if (player_collision_check(player, dx, 0)) {
        player->geometry.x += dx;
    }
    if (player_collision_check(player, 0, dy)) {
        player->geometry.y += dy;
    }
}

Vector2f player_get_relative_position(Player *player, Camera *camera)
{
    Vector2f rel_pos = {player->geometry.x - camera->geometry.x, player->geometry.y - camera->geometry.y};
    return rel_pos;
}

void player_render(Player *player, SDL_Renderer *renderer, Camera *camera)
{
    if (!player->is_alive) {
        return;
    }
    Vector2f rel_pos = player_get_relative_position(player, camera);
    render_texture_ex(player->texture,
                      renderer,
                      (int)rel_pos.x,
                      (int)rel_pos.y,
                      player->geometry.width,
                      player->geometry.height,
                      player->angle);
}

// delta_time in milliseconds
void player_keystates_process(Player *player, const Uint8 *keystates)
{
    if (!player->is_alive) {
        return;
    }
    player->velocity.x = player->velocity.y = 0;
    if (keystates[SDL_SCANCODE_UP] || keystates[SDL_SCANCODE_W])
    {
        /*player->velocity.x += PLAYER_VELOCITY * sin(deg_to_rad(player->angle));*/
        player->velocity.y -= PLAYER_VELOCITY/* * cos(deg_to_rad(player->angle))*/;
    }
    if (keystates[SDL_SCANCODE_DOWN] || keystates[SDL_SCANCODE_S])
    {
        /*player->velocity.x -= PLAYER_VELOCITY * sin(deg_to_rad(player->angle));*/
        player->velocity.y += PLAYER_VELOCITY/* * cos(deg_to_rad(player->angle))*/;
    }
    if (keystates[SDL_SCANCODE_LEFT] || keystates[SDL_SCANCODE_A])
    {
        player->velocity.x -= PLAYER_VELOCITY/* * cos(deg_to_rad(player->angle))*/;
        /*player->velocity.y -= PLAYER_VELOCITY * sin(deg_to_rad(player->angle));*/
    }
    if (keystates[SDL_SCANCODE_RIGHT] || keystates[SDL_SCANCODE_D])
    {
        player->velocity.x += PLAYER_VELOCITY/* * cos(deg_to_rad(player->angle))*/;
        /*player->velocity.y += PLAYER_VELOCITY * sin(deg_to_rad(player->angle));*/
    }
}

void player_angle_process(Player *player, Camera *camera)
{
    if (!player->is_alive) {
        return;
    }
    int mouse_x, mouse_y;
    Vector2f rel_pos = player_get_relative_position(player, camera);
    SDL_GetMouseState(&mouse_x, &mouse_y);
    double x = mouse_x - rel_pos.x - player->geometry.width / 2.0;
    double y = mouse_y - rel_pos.y - player->geometry.height / 2.0;
    player->angle = rad_to_deg(atan2(y, x)) + 90.0;
}

bool player_do_shot(Player *player, Bullets *bullets)
{
    if (!player->is_alive) {
        return false;
    }
    unsigned time = SDL_GetTicks();
    if (time - player->last_shot_time < 1.f / PLAYER_FIRE_RATE * 1000) {
        return false;
    }
    Vector2f bullet_pos = {player->geometry.x + player->geometry.width  / 2.f +
                                   PLAYER_FIRE_START_DISTANCE * (float)sin(deg_to_rad(player->angle)),
                           player->geometry.y + player->geometry.height / 2.f -
                                   PLAYER_FIRE_START_DISTANCE * (float)cos(deg_to_rad(player->angle))};
    if (bullets_add_bullet(bullets, bullet_pos, player->angle)) {
        player->last_shot_time = time;
        return true;
    } else {
        return false;
    }
}

void player_kill(Player *player)
{
    player->is_alive = false;
}

void player_apply_response_this(Player *player, Deque *requests_list, ResponseStructure *response) {
    // Remove all requests with number < response->number
    while (((RequestStructure *) deque_peek_first(requests_list)) != NULL &&
           ((RequestStructure *) deque_peek_first(requests_list))->req_number < response->res_number) {
        request_destroy((RequestStructure *) deque_remove_first(requests_list));
    }
    // Case of loss package
    if (((RequestStructure *) deque_peek_first(requests_list)) == NULL ||
        ((RequestStructure *) deque_peek_first(requests_list))->req_number != response->res_number) {
        return;
    }

    // Save the last shift (server doesn't know about it) and move player to the point in response.
    Vector2f shift_after_last_request = {player->geometry.x - response->players[GlobalVariables.number_of_player].position.x,
                                         player->geometry.y - response->players[GlobalVariables.number_of_player].position.y};
    player_move_to(player,
                   response->players[GlobalVariables.number_of_player].position.x,
                   response->players[GlobalVariables.number_of_player].position.y);
    player->is_alive = response->players[GlobalVariables.number_of_player].is_alive;

    // This request should be deleted, so there is special processing
    RequestStructure *request = (RequestStructure *) deque_remove_first(requests_list);
    // Request number is equal to response number, but there is nothing else to apply
    if (deque_peek_first(requests_list) == NULL) {
        request_destroy(request);
        return;
    } else {
        player_move_on(player,
                       ((RequestStructure *) deque_peek_first(requests_list))->player_state.position.x -
                       request->player_state.position.x,
                       ((RequestStructure *) deque_peek_first(requests_list))->player_state.position.y -
                       request->player_state.position.y);
        request_destroy(request);
    }

    // Apply shift from the requests server doesn't know about
    Iterator it;
    deque_iterator_create(requests_list, &it);
    while (it.data != NULL) {
        RequestStructure *prev = (RequestStructure *) deque_iterator_get_data(&it);
        if (deque_iterator_next(requests_list, &it) == NULL) {
            break;
        }
        player_move_on(player,
                       ((RequestStructure *) deque_iterator_get_data(&it))->player_state.position.x -
                       prev->player_state.position.x,
                       ((RequestStructure *) deque_iterator_get_data(&it))->player_state.position.y -
                       prev->player_state.position.y);
    }
    deque_iterator_destroy(&it);
    player_move_on(player, shift_after_last_request.x, shift_after_last_request.y);
}

void player_apply_response_others(Player players[PLAYER_COUNT], ResponseStructure *response) {
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        if (i == GlobalVariables.number_of_player) {
            continue;
        }
        PlayerStateResponse *res = &response->players[i];
        players[i].angle = res->angle;
        players[i].is_alive = res->is_alive;
        players[i].geometry.x = res->position.x;
        players[i].geometry.y = res->position.y;
        players[i].velocity.x = res->velocity.x;
        players[i].velocity.y = res->velocity.y;
    }
}