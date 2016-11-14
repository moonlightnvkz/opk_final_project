//
// Created by moonlightnvkz on 10.11.16.
//

#include <SDL2/SDL.h>
#include "player.h"
#include "default_values.h"
#include "sdl_helpers.h"

int player_create(Player *player, SDL_Renderer *renderer)
{
    player->position.x = PLAYER_X;
    player->position.y = PLAYER_Y;
    player->velocity.x = PLAYER_VELOCITY_X;
    player->velocity.y = PLAYER_VELOCITY_Y;
    SDL_Surface *bmp = SDL_LoadBMP(PLAYER_TEXTURE);

    player->texture = load_texture(PLAYER_TEXTURE, renderer);
    if (player->texture == NULL) {
        return 1;
    }

    return 0;
}

void player_destroy(Player *player)
{
    SDL_DestroyTexture(player->texture);
}

void player_move_to(Player *player, Vector2i dst)
{
    player->position.x = dst.x;
    player->position.y = dst.y;
}

void player_move_on(Player *player, Vector2i shift)
{
    player->position.x += shift.x;
    player->position.y += shift.y;
}

void player_render(Player *player, SDL_Renderer *renderer)
{
    render_texture(player->texture, renderer, (int)player->position.x, (int)player->position.y);
}

// delta_time in milliseconds
void player_key_process(Player *player, unsigned delta_ticks, SDL_Keycode  key)
{
    switch (key)
    {
        case SDLK_UP:
            player->position.y -= (float)delta_ticks / 100 / FPS * PLAYER_VELOCITY_Y;
            break;
        case SDLK_DOWN:
            player->position.y += (float)delta_ticks / 100 / FPS * PLAYER_VELOCITY_Y;
            break;
        case SDLK_LEFT:
            player->position.x -= (float)delta_ticks / 100 / FPS * PLAYER_VELOCITY_X;
            break;
        case SDLK_RIGHT:
            player->position.x += (float)delta_ticks / 100 / FPS * PLAYER_VELOCITY_X;
            break;
        default:;
    }
}