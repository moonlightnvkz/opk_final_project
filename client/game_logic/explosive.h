//
// Created by moonlightnvkz on 18.12.16.
//

#pragma once

#include <SDL_system.h>
#include "geometry.h"
#include "../default_values.h"

typedef struct tCamera Camera;
typedef struct tPlayer Player;

typedef struct tExplosive {
    Vector2f size;
    Vector2i position_at_map;   // map_descr[][]
    bool is_damaged;
    int timer_damaged;
    bool is_exploding;
    int timer_explosion;
    SDL_Texture *current_sprite;
} Explosive;

typedef struct tExplosives {
    Explosive explosives[EXPLOSIVE_MAX_AMOUNT];
    unsigned number;
    SDL_Texture *texture_damaged[EXPLOSIVE_DAMAGED_SPRITES_COUNT];
    SDL_Texture *texture_explosion[EXPLOSIVE_EXPLOSION_SPRITES_COUNT];
} Explosives;

bool explosives_create(Explosives *explosives, SDL_Renderer *renderer);

void explosives_destroy(Explosives *explosives);

void explosives_render_exposive(Explosive *explosive, SDL_Renderer *renderer, Camera *camera);

Explosive *explosives_get_explosive_on(Explosives *explosives, int w, int h);

void explosive_on_damage(Explosive *explosive);

void explosives_explode_process(Explosives *explosives, unsigned delta_ticks, Player *players);

bool exposives_add_explosive(Explosives *explosives, int w, int h);