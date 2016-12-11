//
// Created by moonlightnvkz on 04.12.16.
//

#pragma once

#include "geometry.h"
#include "pointing_arrow.h"

typedef struct tPlayer Player;

typedef struct tCamera {
    ObjectGeometry geometry;
    PointingArrow arrow;
} Camera;

bool camera_create(Camera *camera, SDL_Renderer *renderer);

void camera_destroy(Camera *camera);

void camera_move_on(Camera *camera, float dx, float dy);

void camera_move_to(Camera *camera, float x, float y);

void camera_move_after_the_player(Camera *camera, Player *player);

void camera_render_pointing_arrow(Camera *camera, SDL_Renderer *renderer, Vector2f point_from, Vector2f point_to);