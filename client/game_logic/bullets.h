//
// Created by moonlightnvkz on 15.11.16.
//

#pragma once

#include <stdlib.h>
#include <SDL_system.h>
#include "geometry.h"

typedef struct tBulletsStateResponse BulletsStateResponse;
typedef struct tCamera Camera;

typedef struct tBullet {
    ObjectGeometry geometry;
    double angle;   // Velocity is the same for all bullets. Calculates according to the angle;
    bool active;
} Bullet;

typedef struct tBullets {
    Bullet *bullets;
    unsigned number;  // Number of active bullets. Memory for all bullets allocates on the start
    SDL_Texture *texture;
} Bullets;

Bullets *bullets_create(SDL_Renderer *renderer);

void bullets_destroy(Bullets *bullets);

void bullets_move_all(Bullets *bullets, unsigned delta_ticks);

void bullet_move(Bullet *bullet, unsigned delta_ticks);

void bullets_render_all(Bullets *bullets, SDL_Renderer *renderer, Camera *camera);

void bullet_render(Bullet *bullet, SDL_Texture *texture, SDL_Renderer *renderer, Camera *camera);

void bullets_apply_response(Bullets *bullets, BulletsStateResponse *response);