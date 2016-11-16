//
// Created by moonlightnvkz on 15.11.16.
//

#pragma once

#include <stdlib.h>
#include <SDL_system.h>
#include "geometry.h"

typedef struct tBullet {
    ObjectGeometry geometry;
    double angle;   // Velocity is the same for all bullets. Calculates according to the angle;
    bool active;
} Bullet;

typedef struct tBullets {
    Bullet *bullets;
    size_t number;  // Number of active bullets. Memory for all bullets allocates on the start
    SDL_Texture *texture;
} Bullets;

Bullets *bullet_create(SDL_Renderer *renderer);

void bullet_destroy(Bullets *bullets);

void bullet_move_all(Bullets *bullets, unsigned delta_ticks);

void bullet_move(Bullet *bullet, unsigned delta_ticks);

void bullet_render_all(Bullets *bullets, SDL_Renderer *renderer);

void bullet_render(Bullet *bullet, SDL_Texture *texture, SDL_Renderer *renderer);