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

    if (!explosives_create(&map->explosives)) {
        LOG_ERROR("Failed to create explosives");
        tilemap_destroy(map);
        return false;
    }

    for (unsigned w = 0; w < map->map_descr.width; ++w)
    {
        for (unsigned h = 0; h < map->map_descr.height; ++h)
        {
            if (map->map_descr.map_descr[h][w] == TM_EXPLOSIVE) {
                exposives_add_explosive(&map->explosives, w, h);
            }
        }
    }
    GlobalVariables.map = map;
    return true;
}

void tilemap_destroy(TileMap *map)
{
    explosives_destroy(&map->explosives);
}

bool tilemap_load_from_file(TileMap *map, const char * file_path)
{
    FILE *file = fopen(file_path, "r");
    if (!file) {
        LOG_ERROR("Failed to open tilemap file");
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
    unsigned tile_x1, tile_x2, tile_y1, tile_y2;

    tilemap_coords_to_tiles(geom, &tile_x1, &tile_x2, &tile_y1, &tile_y2);

    MapDescription *map_descr = GlobalVariables.map_descr;
    for (unsigned i = tile_x1; i <= tile_x2; ++i) {
        for (unsigned j = tile_y1; j <= tile_y2; ++j) {
            if (map_descr->map_descr[j][i] == TM_BLOCK) {
                return false;
            }
            if (map_descr->map_descr[j][i] == TM_EXPLOSIVE) {
                explosive_on_damage(explosives_get_explosive_on(&GlobalVariables.map->explosives, i, j));
                return false;
            }
        }
    }
    return true;
}

void tilemap_coords_to_tiles(ObjectGeometry geom, unsigned *x1, unsigned *x2, unsigned *y1, unsigned *y2)
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
    *x1 = (unsigned) tile_x1;
    *x2 = (unsigned) tile_x2;
    *y1 = (unsigned) tile_y1;
    *y2 = (unsigned) tile_y2;
}