//
// Created by moonlightnvkz on 15.11.16.
//

#pragma once

#include <stdlib.h>
#include <SDL_system.h>
#include "geometry.h"
#include "../default_values.h"

typedef struct tBullet {
    ObjectGeometry geometry;
    double angle;   // Velocity is the same for all bullets. Calculates according to the angle;
    int ttl;
} Bullet;

typedef struct tBullets {
    Bullet bullets[BULLET_MAX_AMOUNT];
    unsigned number;  // Number of active bullets. Memory for all bullets allocates on the start
} Bullets;

Bullets *bullets_create();

void bullets_destroy(Bullets *bullets);

void bullets_move_all(Bullets *bullets, unsigned delta_ticks);

void bullet_move(Bullet *bullet, unsigned delta_ticks);

bool bullets_add_bullet(Bullets *bullets, Vector2f position, double angle);