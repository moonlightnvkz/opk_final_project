//
// Created by moonlightnvkz on 05.12.16.
//

#include "tile_map.h"
#include "../loggers.h"
#include "../default_values.h"
#include "sdl_helpers.h"
#include "camera.h"

TileMap *tilemap_create(SDL_Renderer *renderer)
{
    TileMap *m = malloc(sizeof(TileMap));
    if (!m) {
        log_error("Failed to allocate memory for TileMap", __FUNCTION__, __LINE__);
        return NULL;
    }
    m->size.x = MAP_WIDTH;
    m->size.y = MAP_HEIGHT;

    m->tile.size.x = TILE_WIDTH;
    m->tile.size.y = TILE_HEIGHT;

    m->tile.texture = load_texture(TILE_TEXTURE, renderer);
    if (!m->tile.texture) {
        log_error("Failed to load texture for TileMap's tile", __FUNCTION__, __LINE__);
        tilemap_destroy(m);
        return NULL;
    }
    return m;
}

void tilemap_destroy(TileMap *map)
{
    SDL_DestroyTexture(map->tile.texture);
    free(map);
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