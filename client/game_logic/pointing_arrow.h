//
// Created by moonlightnvkz on 11.12.16.
//

#pragma once

#include <SDL_system.h>
#include "geometry.h"

typedef struct tCamera Camera;
typedef struct tPlayer Player;


typedef struct tPointingArrow {
    Vector2i size;
    SDL_Texture *texture;
} PointingArrow;

bool pa_create(PointingArrow *arrow, SDL_Renderer *renderer);

void pa_destroy(PointingArrow *arrow);

void pa_render(PointingArrow *arrow, Camera *camera, SDL_Renderer *renderer, Vector2f point_from, Vector2f point_to);