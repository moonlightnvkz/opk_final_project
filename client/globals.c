//
// Created by moonlightnvkz on 24.12.16.
//

#include "globals.h"
#include "defines.h"
#include "game_logic/tile_map.h"

bool globals_init()
{
    GlobalVariables.quit = false;
    GlobalVariables.number_of_the_player = 0;
    return true;
}