//
// Created by moonlightnvkz on 18.12.16.
//

#pragma once

#include <SDL_system.h>
#include "geometry.h"
#include "../default_values.h"

typedef struct tCamera Camera;

typedef struct tTile {
    SDL_Texture *texture;
} Tile;

typedef struct tMapDescription {
    unsigned width;  // = MAP_WIDTH / TILE_WIDTH
    unsigned height; // = MAP_HEIGHT / TILE_HEIGHT
    int map_descr[MAP_WIDTH / TILE_WIDTH][MAP_HEIGHT / TILE_HEIGHT];
} MapDescription;

typedef struct tTileMap {
    Vector2i size;
    MapDescription map_descr;
    Tile tiles[MAP_TILE_NUMBER];
    Vector2i tile_size;
} TileMap;

bool tilemap_create(TileMap *map);

void tilemap_destroy(TileMap *map);

bool tilemap_collision_check(ObjectGeometry geom);