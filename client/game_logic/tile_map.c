//
// Created by moonlightnvkz on 05.12.16.
//

#include <assert.h>
#include "tile_map.h"
#include "../loggers.h"
#include "../default_values.h"
#include "sdl_helpers.h"
#include "camera.h"

bool tilemap_create(TileMap *map, SDL_Renderer *renderer)
{
    assert(map != NULL);
    map->size.x = MAP_WIDTH;
    map->size.y = MAP_HEIGHT;

    map->tile.size.x = TILE_WIDTH;
    map->tile.size.y = TILE_HEIGHT;

    map->tile.texture = load_texture(TILE_TEXTURE, renderer);
    if (!map->tile.texture) {
        log_error("Failed to load texture for TileMap's tile", __FUNCTION__, __LINE__);
        map->tile.texture = load_texture(MISSING_TEXTURE, renderer);
        if (map->tile.texture == NULL) {
            log_error("Failed to load <missing_texture>", __FUNCTION__, __LINE__);
            return false;
        }
    }
    return true;
}

void tilemap_destroy(TileMap *map)
{
    SDL_DestroyTexture(map->tile.texture);
}

void tilemap_render(TileMap *map, SDL_Renderer *renderer, Camera *camera) {
    int x = (int) ((int) camera->geometry.x / map->tile.size.x * map->tile.size.x - camera->geometry.x);
    int y = (int) ((int) camera->geometry.y / map->tile.size.y * map->tile.size.y - camera->geometry.y);
    for (int w = x; w < camera->geometry.x + camera->geometry.width; w += map->tile.size.x) {
        for (int h = y; h < camera->geometry.y + camera->geometry.height; h += map->tile.size.y) {
            render_texture(map->tile.texture, renderer, w, h, map->tile.size.x, map->tile.size.y);
        }
    }
}