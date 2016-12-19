//
// Created by moonlightnvkz on 18.12.16.
//

#pragma once

#include <SDL_system.h>
#include "geometry.h"
#include "../default_values.h"
#include "explosives.h"

typedef struct tCamera Camera;

typedef enum eTileTypes {
    TM_FLOOR,
    TM_BLOCK,
    TM_EXPLOSIVE
} TileTypes;

typedef struct tMapDescription {
    unsigned width;  // = MAP_WIDTH / TILE_WIDTH
    unsigned height; // = MAP_HEIGHT / TILE_HEIGHT
    int map_descr[MAP_WIDTH / TILE_WIDTH][MAP_HEIGHT / TILE_HEIGHT];
} MapDescription;

typedef struct tTileMap {
    Vector2i size;
    MapDescription map_descr;
    Vector2i tile_size;
    Explosives explosives;
} TileMap;

bool tilemap_create(TileMap *map);

void tilemap_destroy(TileMap *map);

void tilemap_coords_to_tiles(ObjectGeometry geom, unsigned *x1, unsigned *x2, unsigned *y1, unsigned *y2);

bool tilemap_collision_check(ObjectGeometry geom);