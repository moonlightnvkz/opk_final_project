//
// Created by moonlightnvkz on 10.11.16.
//

#include <math.h>
#include <SDL_mouse.h>
#include <SDL_timer.h>
#include "player.h"
#include "bullets.h"
#include "sdl_helpers.h"
#include "../default_values.h"
#include "../loggers.h"
#include "../server_logic/request_response.h"
#include "../my_deque.h"
#include "camera.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846	/* pi */
#endif

static void player_move_on(Player *player, float dx, float dy);

static void player_move_to(Player *player, float x, float y);

Player *player_create(SDL_Renderer *renderer)
{
    Player *player = malloc(sizeof(Player));
    if (player == NULL) {
        log_error("Failed to allocate memory for this_player", __FUNCTION__, __LINE__);
        exit(1);
    }
    player->geometry.x = PLAYER_X;
    player->geometry.y = PLAYER_Y;
    player->geometry.width = PLAYER_WIDTH;
    player->geometry.height = PLAYER_HEIGHT;
    player->angle = 0.0;
    player->velocity.x = 0;
    player->velocity.y = 0;
    player->shot_done = false;
    player->last_shot_time = 0;
    player->texture = load_texture(PLAYER_TEXTURE, renderer);
    if (player->texture == NULL) {
        log_error("Failed to load texture", __FUNCTION__, __LINE__);
        player->texture = load_texture(MISSING_TEXTURE, renderer);
        if (player->texture == NULL) {
            log_error("Failed to load <missing_texture>", __FUNCTION__, __LINE__);
            return NULL;
        }
    }

    return player;
}

void player_destroy(Player *player)
{
    SDL_DestroyTexture(player->texture);
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

static bool player_collision_check(Player *player, float dx, float dy)
{
    return true;
    Vector2f new_pos = {player->geometry.x + dx, player->geometry.y + dy};
    Vector2i dimensions = {player->geometry.width, player->geometry.height};
    if (new_pos.x < 0 ||
        new_pos.y < 0 ||
        new_pos.x + dimensions.x > WINDOW_WIDTH ||
        new_pos.y + dimensions.y > WINDOW_HEIGHT)
    {
        return false;
    } else {
        return true;
    }
}

static void player_move_on(Player *player, float dx, float dy)
{
    if (!player_collision_check(player, dx, dy)) {
        return;
    }
    player->geometry.x += dx;
    player->geometry.y += dy;
}

Vector2f player_get_relative_position(Player *player, Camera *camera)
{
    Vector2f rel_pos = {player->geometry.x - camera->geometry.x, player->geometry.y - camera->geometry.y};
    return rel_pos;
}

void player_render(Player *player, SDL_Renderer *renderer, Camera *camera)
{
    Vector2f rel_pos = player_get_relative_position(player, camera);
    render_texture_ex(player->texture, renderer,
                      (int)rel_pos.x,(int)rel_pos.y,
                      player->geometry.width, player->geometry.height, player->angle);
}

// delta_time in milliseconds
void player_keystates_process(Player *player, const Uint8 *keystates)
{
    player->velocity.x = player->velocity.y = 0;
    if (keystates[SDL_SCANCODE_UP])
    {
        player->velocity.y -= PLAYER_VELOCITY;
    }
    if (keystates[SDL_SCANCODE_DOWN])
    {
        player->velocity.y += PLAYER_VELOCITY;
    }
    if (keystates[SDL_SCANCODE_LEFT])
    {
        player->velocity.x -= PLAYER_VELOCITY;
    }
    if (keystates[SDL_SCANCODE_RIGHT])
    {
        player->velocity.x += PLAYER_VELOCITY;
    }
}

void player_angle_process(Player *player, Camera *camera)
{
    int mouse_x, mouse_y;
    Vector2f rel_pos = player_get_relative_position(player, camera);
    SDL_GetMouseState(&mouse_x, &mouse_y);
    double x = mouse_x - rel_pos.x - player->geometry.width / 2.0;
    double y = mouse_y - rel_pos.y - player->geometry.height / 2.0;
    player->angle = (int)(atan2(y, x) / M_PI * 180) + 90;
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
    player->shot_done = true;
    bullets->bullets[bullets->number].angle = player->angle;
    bullets->bullets[bullets->number].geometry.x = player->geometry.x + player->geometry.width / 2;
    bullets->bullets[bullets->number].geometry.y = player->geometry.y + player->geometry.height / 2;
    bullets->bullets[bullets->number].active = true;
    bullets->number++;
    player->last_shot_time = time;
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
    Vector2f shift_after_last_request = {player->geometry.x - response->this_player_state.position.x,
                                         player->geometry.y - response->this_player_state.position.y};
    player_move_to(player,
                   response->this_player_state.position.x,
                   response->this_player_state.position.y);

    // This request should be deleted, so there is special processing
    RequestStructure *request = (RequestStructure *) deque_remove_first(requests_list);
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

    // Apply shift from the request server doesn't know about
    Iterator *it = deque_iterator_create(requests_list);
    while (it->data != NULL) {
        RequestStructure *prev = (RequestStructure *) deque_iterator_get_data(it);
        if (deque_iterator_next(requests_list, it) == NULL) {
            break;
        }
        player_move_on(player,
                       ((RequestStructure *) deque_iterator_get_data(it))->player_state.position.x -
                       prev->player_state.position.x,
                       ((RequestStructure *) deque_iterator_get_data(it))->player_state.position.y -
                       prev->player_state.position.y);
    }
    player_move_on(player, shift_after_last_request.x, shift_after_last_request.y);
}

void player_apply_response_diff(Player *player, ResponseStructure *response) {
    PlayerStateResponse *response_diff = &response->diff_player_state;
    player->angle = response_diff->angle;
    player->geometry.x = response_diff->position.x;
    player->geometry.y = response_diff->position.y;
    player->velocity.x = response_diff->velocity.x;
    player->velocity.y = response_diff->velocity.y;
}