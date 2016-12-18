//
// Created by moonlightnvkz on 05.12.16.
//

#pragma once

#include <SDL_system.h>
#include "geometry.h"
#include "../default_values.h"
#include "explosive.h"

typedef struct tCamera Camera;

typedef enum eTileTypes {
    TM_FLOOR,
    TM_BLOCK,
    TM_EXPLOSIVE
} TileTypes;

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
    Explosives explosives;
} TileMap;

bool tilemap_create(TileMap *map, SDL_Renderer *renderer);

void tilemap_destroy(TileMap *map);

bool tilemap_collision_check(ObjectGeometry geom);

void tilemap_render(TileMap *map, SDL_Renderer *renderer, Camera *camera);