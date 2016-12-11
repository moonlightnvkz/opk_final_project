//
// Created by moonlightnvkz on 05.12.16.
//

#pragma once

#include <SDL_system.h>
#include "geometry.h"

typedef struct tCamera Camera;

typedef struct tTile {
    Vector2i size;
    SDL_Texture *texture;
} Tile;

typedef struct tTileMap {
    Vector2i size;
    // Only one tile will be rendered to fill all the map
    Tile tile;
} TileMap;

bool tilemap_create(TileMap *map, SDL_Renderer *renderer);

void tilemap_destroy(TileMap *map);

void tilemap_render(TileMap *map, SDL_Renderer *renderer, Camera *camera);