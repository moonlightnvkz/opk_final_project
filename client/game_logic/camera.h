//
// Created by moonlightnvkz on 04.12.16.
//

#pragma once

#include "geometry.h"

typedef struct tPlayer Player;

typedef struct tCamera {
    ObjectGeometry geometry;
} Camera;

Camera *camera_create();

void camera_destroy(Camera *camera);

void camera_move_on(Camera *camera, float dx, float dy);

void camera_move_to(Camera *camera, float x, float y);

void camera_move_after_the_player(Camera *camera, Player *player);