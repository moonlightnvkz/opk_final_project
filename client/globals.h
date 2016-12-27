//
// Created by moonlightnvkz on 24.12.16.
//

#pragma once

#include <stdbool.h>
#include "game_logic/geometry.h"

typedef struct tMapDescription MapDescription;
typedef struct tTileMap TileMap;

struct tGlobalVariables {
    bool quit;
    unsigned number_of_the_player;
} GlobalVariables;

bool globals_init();