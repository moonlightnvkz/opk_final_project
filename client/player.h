//
// Created by moonlightnvkz on 10.11.16.
//

#pragma once

#include <stdio.h>
#include <SDL2/SDL.h>
#include "geometry.h"

typedef struct tPlayer {
    Vector2f position;
    SDL_Texture *texture;
    Vector2i velocity;
} Player;

int player_create(Player *player, SDL_Renderer *renderer);

void player_destroy(Player *player);

void player_key_process(Player *player, unsigned delta_ticks, SDL_Keycode key);

void player_move_to(Player *player, Vector2i dest);

void player_move_on(Player *player, Vector2i shift);

void player_render(Player *player, SDL_Renderer *renderer);