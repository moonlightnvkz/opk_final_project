//
// Created by moonlightnvkz on 15.11.16.
//

#pragma once

#include <stdlib.h>
#include "geometry.h"

typedef struct tBullet {
    Vector2f position;
    double angle;   // Velocity is the same for all bullets. Calculates according to the angle;
} Bullet;

void bullet_move_all(Bullet *bullets, size_t number, unsigned delta_ticks);

void bullet_move(Bullet *bullet, unsigned delta_ticks);