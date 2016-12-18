//
// Created by moonlightnvkz on 18.12.16.
//

#include <assert.h>
#include "tile_map.h"
#include "../loggers.h"

bool tilemap_load_from_file(TileMap *map, const char * file_path);

bool tilemap_create(TileMap *map)
{
    assert(map != NULL);
    map->size.x = MAP_WIDTH;
    map->size.y = MAP_HEIGHT;

    map->map_descr.width = MAP_WIDTH / TILE_WIDTH;
    map->map_descr.height = MAP_HEIGHT / TILE_HEIGHT;

    map->tile_size.x = TILE_WIDTH;
    map->tile_size.y = TILE_HEIGHT;

    GlobalVariables.tile_size.x = TILE_WIDTH;
    GlobalVariables.tile_size.y = TILE_HEIGHT;

    if (!tilemap_load_from_file(map, MAP_LOAD_FILE)) {
        return false;
    }

    GlobalVariables.map_descr = &map->map_descr;
    return true;
}

void tilemap_destroy(TileMap *map)
{
    return;
}

bool tilemap_load_from_file(TileMap *map, const char * file_path)
{
    FILE *file = fopen(file_path, "r");
    if (!file) {
        log_error("Failed to open tilemap file", __FUNCTION__, __LINE__);
        return false;
    }
    for (unsigned h = 0; h < map->map_descr.height; ++h){
        for (unsigned w = 0; w < map->map_descr.width; ++w) {
            if (fscanf(file, "%d", &map->map_descr.map_descr[h][w]) == 0) {
                return false;
            }
        }
    }
    fclose(file);
    return true;
}

bool tilemap_collision_check(ObjectGeometry geom)
{
    MapDescription *map_descr = GlobalVariables.map_descr;
    int tile_x1 = (int)  geom.x / GlobalVariables.tile_size.x;
    int tile_y1 = (int)  geom.y / GlobalVariables.tile_size.y;
    int tile_x2 = (int) (geom.x + geom.width) / GlobalVariables.tile_size.x;
    int tile_y2 = (int) (geom.y + geom.height) / GlobalVariables.tile_size.y;

    tile_x1 < 0 ? (tile_x1 = 0) : 0;
    tile_x2 < 0 ? (tile_x2 = 0) : 0;
    tile_y1 < 0 ? (tile_y1 = 0) : 0;
    tile_y2 < 0 ? (tile_y2 = 0) : 0;
    tile_x1 >= map_descr->width ? (tile_x1 = map_descr->width  - 1) : 0;
    tile_x2 >= map_descr->width ? (tile_x2 = map_descr->width  - 1) : 0;
    tile_y1 >= map_descr->width ? (tile_y1 = map_descr->height - 1) : 0;
    tile_y2 >= map_descr->width ? (tile_y2 = map_descr->height - 1) : 0;

    for (int i = tile_x1; i <= tile_x2; ++i) {
        for (int j = tile_y1; j <= tile_y2; ++j) {
            if (map_descr->map_descr[j][i] != 0) {
                return false;
            }
        }
    }
    return true;
}