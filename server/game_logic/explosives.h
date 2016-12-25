//
// Created by moonlightnvkz on 19.12.16.
//

#pragma once

#include "geometry.h"
#include "../defines.h"

typedef struct tPlayer Player;
typedef struct tTileMap TileMap;

typedef struct tExplosive {
    Vector2f size;
    Vector2i position_at_map;   // map_descr[][]
    bool is_damaged;
    int timer_damaged;
    bool is_exploding;
    int timer_explosion;
} Explosive;

typedef struct tExplosives {
    Explosive explosives[EXPLOSIVE_MAX_AMOUNT];
    unsigned number;
} Explosives;

bool explosives_create(Explosives *explosives);

void explosives_destroy(Explosives *explosives);

Explosive *explosives_get_explosive_on(Explosives *explosives, int w, int h);

void explosive_on_damage(Explosive *explosive);

void explosives_explode_process(Explosives *explosives, unsigned delta_ticks, TileMap *map, Player *players);

bool exposives_add_explosive(Explosives *explosives, int w, int h);