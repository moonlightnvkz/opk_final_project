//
// Created by moonlightnvkz on 10.11.16.
//

#pragma once

#include <stdio.h>
#include <SDL_system.h>
#include "geometry.h"

typedef struct tPlayer {
    ObjectGeometry geometry;
    SDL_Texture *texture;
    double angle;
    Vector2i velocity;
} Player;

int player_create(Player *player, SDL_Renderer *renderer);

void player_destroy(Player *player);

void player_keystates_process(Player *player, const Uint8 *keystates);

void player_move(Player *player, unsigned delta_ticks);

void player_render(Player *player, SDL_Renderer *renderer);