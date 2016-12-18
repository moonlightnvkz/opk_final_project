//
// Created by moonlightnvkz on 11.12.16.
//

#include "default_values.h"
#include "game_logic/tile_map.h"

void globals_init()
{
    GlobalVariables.quit = false;
    GlobalVariables.number_of_player = 0;
    GlobalVariables.map_geometry.x = 0;
    GlobalVariables.map_geometry.y = 0;
    GlobalVariables.map_geometry.width = MAP_WIDTH;
    GlobalVariables.map_geometry.height = MAP_HEIGHT;
    GlobalVariables.map_descr = NULL;
    GlobalVariables.tile_size.x = 0;
    GlobalVariables.tile_size.y = 0;
    GlobalVariables.map = NULL;
}