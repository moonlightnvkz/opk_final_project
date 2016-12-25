//
// Created by moonlightnvkz on 24.12.16.
//

#pragma once

#include <stdbool.h>
#include "game_logic/geometry.h"

typedef struct tMapDescription MapDescription;
typedef struct tTileMap TileMap;

struct tGlobalVariables {
    //ObjectGeometry map_geometry;
    //Vector2i tile_size;
    //TileMap *map;
    //MapDescription *map_descr;
    unsigned players_alive_to_end_the_game;
} GlobalVariables;

bool globals_init();