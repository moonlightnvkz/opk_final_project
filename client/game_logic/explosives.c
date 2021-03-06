//
// Created by moonlightnvkz on 18.12.16.
//

#include <assert.h>
#include <string.h> //memcpy
#include "explosives.h"
#include "sdl_helpers.h"
#include "../loggers.h"
#include "camera.h"
#include "tile_map.h"
#include "../server_logic/request_response.h"
#include "player.h"
#include "../globals.h"

void explosive_disable_explosive(Explosives *explosives, unsigned w, unsigned h, TileMap *map);

bool explosives_create(Explosives *explosives, SDL_Renderer *renderer)
{
    assert(explosives != NULL);
    explosives->number = 0;
    for (unsigned i = 0; i < EXPLOSIVE_MAX_AMOUNT; ++i) {
        Explosive *explosive = &explosives->explosives[i];
        explosive->position_at_map.x = explosive->position_at_map.y = 0;
        explosive->size.x = EXPLOSIVE_WIDTH;
        explosive->size.y = EXPLOSIVE_HEIGHT;
        explosive->is_damaged = explosive->is_exploding = 0;
        explosive->timer_damaged = explosive->timer_explosion = 0;
        explosive->current_sprite = explosives->texture_damaged[0];
    }
    {
        char texture_path[] = EXPLOSIVE_DAMAGED_TEXTURE;
        for (unsigned i = 0; i < EXPLOSIVE_DAMAGED_SPRITES_COUNT; ++i) {
            texture_path[EXPLOSIVE_DAMAGED_NUMBER_IDX] = (char) (i + '0');
            SDL_Texture **texture = &explosives->texture_damaged[i];
            *texture = load_texture(texture_path, renderer);
            if (!*texture) {
                LOG_ACTION(Failed to load texture for Explosives damaged sprite);
                *texture = load_texture(MISSING_TEXTURE, renderer);
                if (!*texture) {
                    LOG_ACTION(Failed to load <missing_texture>);
                    return false;
                }
            }
        }
    }
    {
        char texture_path[] = EXPLOSIVE_EXPLOSION_TEXTURE;
        for (unsigned i = 0; i < EXPLOSIVE_EXPLOSION_SPRITES_COUNT; ++i) {
            texture_path[EXPLOSIVE_EXPLOSION_NUMBER_IDX] = (char) (i + '0');
            SDL_Texture **texture = &explosives->texture_explosion[i];
            *texture = load_texture(texture_path, renderer);
            if (!*texture) {
                LOG_ACTION(Failed to load texture for Explosives damaged sprite);
                *texture = load_texture(MISSING_TEXTURE, renderer);
                if (!*texture) {
                    LOG_ACTION(Failed to load <missing_texture>);
                    return false;
                }
            }
        }
    }
    return true;
}

void explosives_destroy(Explosives *explosives)
{
    for (unsigned i = 0; i < EXPLOSIVE_DAMAGED_SPRITES_COUNT; ++i) {
        SDL_DestroyTexture(explosives->texture_damaged[i]);
    }
    for (unsigned i = 0; i < EXPLOSIVE_EXPLOSION_SPRITES_COUNT; ++i) {
        SDL_DestroyTexture(explosives->texture_explosion[i]);
    }
}

void explosive_on_damage(Explosive *explosive)
{
    if (explosive == NULL || explosive->is_damaged || explosive->is_exploding) {
        return;
    }
    explosive->is_damaged = true;
    explosive->timer_damaged = EXPLOSIVE_DAMAGED_ANIMATION_TIME;
}

bool explosive_need_disable(Explosive *explosive) {
    return explosive->is_exploding && explosive->timer_explosion == 0;
}

void explosives_explode_process(Explosives *explosives, unsigned delta_ticks, TileMap *map, Player *players)
{
    for (unsigned i = 0; i < explosives->number; ++i) {
        Explosive *explosive = &explosives->explosives[i];
        // Case it's undamaged
        if (!explosive->is_damaged) {
            explosive->current_sprite = explosives->texture_damaged[0];
            continue;
        }
        // Animation is over, need to remove it from the map
        if (explosive_need_disable(explosive)) {
            explosive_disable_explosive(explosives, (unsigned)explosive->position_at_map.x, (unsigned)explosive->position_at_map.y, map);
            continue;
        }
        // Explosion case
        if (explosive->timer_damaged == 0 && !explosive->is_exploding) {
            if (explosive->timer_explosion == 0) {
                continue;
            }
            for (unsigned k = 0; k < PLAYER_COUNT; ++k) {
                unsigned x1, x2, y1, y2;
                tilemap_coords_to_tiles(map, players[k].geometry, &x1, &x2, &y1, &y2);
                ObjectGeometry rect1 = {x1, y1, x2 - x1, y2 - y1};
                ObjectGeometry rect2 = {explosive->position_at_map.x - 1, explosive->position_at_map.y - 1, 2, 2};
                if (!geometry_rect_rect_collision_check(rect1, false, rect2)) {
                    player_kill(&players[k]);
                }
            }
            explosive->is_exploding = true;
            explosive->current_sprite = explosives->texture_explosion[0];
            continue;
        }
        // Explosion hasn't started, but need to be started now
        if (!explosive->is_exploding) {
            explosive->timer_damaged -= delta_ticks;
            explosive->timer_damaged < 0 ? (explosive->timer_damaged = 0) : 0;
            explosive->current_sprite = explosives->texture_damaged[explosive->timer_damaged /
                                                                    (EXPLOSIVE_DAMAGED_ANIMATION_TIME /
                                                                     EXPLOSION_DAMAGED_TICKS_IN_ANIMATION) %
                                                                    EXPLOSIVE_DAMAGED_SPRITES_COUNT];
        }
        // Explosion is going
        else {
            explosive->timer_explosion -= delta_ticks;
            explosive->timer_explosion < 0 ? (explosive->timer_explosion = 0) : 0;
            explosive->current_sprite = explosives->texture_explosion[explosive->timer_explosion /
                                                                      (EXPLOSIVE_EXPLOSION_ANIMATION_TIME /
                                                                       EXPLOSIVE_EXPLOSION_SPRITES_COUNT)];
        }
    }
}

void explosives_swap(Explosive *e1, Explosive *e2)
{
    Explosive temp = *e1;
    memcpy(e1, e2, sizeof(Explosive));
    memcpy(e2, &temp, sizeof(Explosive));
}

void explosive_disable_explosive(Explosives *explosives, unsigned w, unsigned h, TileMap *map)
{
    for (unsigned i = 0; i < EXPLOSIVE_MAX_AMOUNT; ++i) {
        if (explosives->explosives[i].position_at_map.x == w &&
                explosives->explosives[i].position_at_map.y == h) {
            map->map_descr.map_descr[h][w] = TM_FLOOR;
            if (i < explosives->number) {
                explosives->number--;
                explosives_swap(explosives->explosives + i, explosives->explosives + explosives->number);
            }
        }
    }
}

bool exposives_add_explosive(Explosives *explosives, int w, int h)
{
    if (explosives->number == EXPLOSIVE_MAX_AMOUNT) {
        return false;
    }
    Explosive *explosive = &explosives->explosives[explosives->number];
    explosive->is_damaged = explosive->is_exploding = false;
    explosive->current_sprite = explosives->texture_damaged[0];
    explosive->position_at_map.x = w;
    explosive->position_at_map.y = h;
    explosive->timer_damaged = EXPLOSIVE_DAMAGED_ANIMATION_TIME;
    explosive->timer_explosion = EXPLOSIVE_EXPLOSION_ANIMATION_TIME;
    explosives->number++;
    return true;
}

Explosive *explosives_get_explosive_at(Explosives *explosives, int w, int h)
{
    for (unsigned i = 0; i < explosives->number; ++i) {
        if (explosives->explosives[i].position_at_map.x == w && explosives->explosives[i].position_at_map.y == h) {
            return &explosives->explosives[i];
        }
    }
    return NULL;
}

void explosives_render_exposive(Explosive *explosive, SDL_Renderer *renderer, Camera *camera, TileMap *map)
{
    if (explosive == NULL) {
        return;
    }
    int x = explosive->position_at_map.x * map->tile_size.x - (int) camera->geometry.x;
    int y = explosive->position_at_map.y * map->tile_size.y - (int) camera->geometry.y;
    render_texture(explosive->current_sprite, renderer, x, y,
                   (int) explosive->size.x,
                   (int) explosive->size.y);
}

void explosives_apply_response(Explosives *explosives, ExplosivesStateResponse *state, TileMap *map)
{
    explosives->number = state->number;
    for (unsigned i = 0; i < EXPLOSIVE_MAX_AMOUNT; ++i) {
        Explosive *explosive = &explosives->explosives[i];
        explosive->position_at_map.x = state->explosives[i].position_at_map.x;
        explosive->position_at_map.y = state->explosives[i].position_at_map.y;
        explosive->is_damaged = state->explosives[i].is_damaged;
        explosive->is_exploding = state->explosives[i].is_exploding;
        explosive->timer_damaged = state->explosives[i].timer_damaged;
        explosive->timer_explosion = state->explosives[i].timer_explosion;
        if (explosive_need_disable(explosive)) {
            explosive_disable_explosive(explosives, (unsigned)explosive->position_at_map.x, (unsigned)explosive->position_at_map.y, map);
        }
    }
}