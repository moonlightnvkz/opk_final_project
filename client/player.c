//
// Created by moonlightnvkz on 10.11.16.
//

#include <math.h>
#include "player.h"
#include "default_values.h"
#include "sdl_helpers.h"
#include "loggers.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846	/* pi */
#endif

static void player_move_on(Player *player, float x, float y);

static void player_move_to(Player *player, float x, float y);

Player *player_create(SDL_Renderer *renderer)
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

void player_render(Player *player, SDL_Renderer *renderer)
{
    render_texture_ex(player->texture, renderer,
                   (int)player->geometry.x,(int)player->geometry.y,
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

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    double x = mouse_x - player->geometry.x - player->geometry.width / 2.0;
    double y = mouse_y - player->geometry.y - player->geometry.height / 2.0;
    player->angle = (int)(atan2(y, x) / M_PI * 180) + 90;
}

void player_do_shot(Player *player, Bullets *bullets)
{
    if (bullets->number == BULLET_MAX_AMOUNT) {
        return;
    }
    bullets->bullets[bullets->number].angle = player->angle;
    bullets->bullets[bullets->number].geometry.x = player->geometry.x + player->geometry.width / 2;
    bullets->bullets[bullets->number].geometry.y = player->geometry.y + player->geometry.height / 2;
    bullets->bullets[bullets->number].active = true;
    bullets->number++;
}