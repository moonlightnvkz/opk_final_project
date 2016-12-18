//
// Created by moonlightnvkz on 18.12.16.
//

#include <assert.h>
#include "explosive.h"
#include "sdl_helpers.h"
#include "../loggers.h"
#include "camera.h"
#include "tile_map.h"

void explosions_disable_explosive(Explosives *explosives, Explosive *explosive);

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
                log_error("Failed to load texture for Explosives damaged sprite", __FUNCTION__, __LINE__);
                *texture = load_texture(MISSING_TEXTURE, renderer);
                if (!*texture) {
                    log_error("Failed to load <missing_texture>", __FUNCTION__, __LINE__);
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
                log_error("Failed to load texture for Explosives damaged sprite", __FUNCTION__, __LINE__);
                *texture = load_texture(MISSING_TEXTURE, renderer);
                if (!*texture) {
                    log_error("Failed to load <missing_texture>", __FUNCTION__, __LINE__);
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
    if (explosive->is_damaged) {
        return;
    }
    explosive->is_damaged = true;
    explosive->timer_damaged = EXPLOSIVE_DAMAGED_ANIMATION_TIME;
}

void explosives_explode_process(Explosives *explosives, unsigned delta_ticks, Player *players)
{
    for (unsigned i = 0; i < explosives->number; ++i) {
        Explosive *explosive = &explosives->explosives[i];
        if (!explosive->is_damaged) {
            continue;
        }
        if (explosive->is_exploding && explosive->timer_explosion == 0) {
            explosions_disable_explosive(explosives, explosive);
        }
        if (explosive->timer_damaged == 0 && !explosive->is_exploding) {
            if (explosive->timer_explosion == 0) {
                continue;
            }
            explosive->is_exploding = true;
            explosive->current_sprite = explosives->texture_explosion[0];
            continue;
        }
        if (!explosive->is_exploding) {
            explosive->timer_damaged -= delta_ticks;
            explosive->timer_damaged < 0 ? (explosive->timer_damaged = 0) : 0;
            explosive->current_sprite = explosives->texture_damaged[explosive->timer_damaged /
                                                                    (EXPLOSIVE_DAMAGED_ANIMATION_TIME /
                                                                     EXPLOSION_DAMAGED_TICKS_IN_ANIMATION) %
                                                                    EXPLOSIVE_DAMAGED_SPRITES_COUNT];
        } else {
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

void explosions_disable_explosive(Explosives *explosives, Explosive *explosive)
{
    for (unsigned i = 0; i < explosives->number; ++i) {
        if (&explosives->explosives[i] == explosive) {
            GlobalVariables.map->map_descr.map_descr[explosive->position_at_map.y][explosive->position_at_map.x] = TM_FLOOR;
            explosives->number--;
            explosives_swap(explosives->explosives + i, explosives->explosives + explosives->number);
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

Explosive *explosives_get_explosive_on(Explosives *explosives, int w, int h)
{
    for (unsigned i = 0; i < explosives->number; ++i) {
        if (explosives->explosives[i].position_at_map.x == w || explosives->explosives[i].position_at_map.y == h) {
            return &explosives->explosives[i];
        }
    }
    return NULL;
}

void explosives_render_exposive(Explosive *explosive, SDL_Renderer *renderer, Camera *camera)
{
    if (explosive == NULL) {
        return;
    }
        int x = explosive->position_at_map.x * GlobalVariables.tile_size.x - (int) camera->geometry.x;
        int y = explosive->position_at_map.y * GlobalVariables.tile_size.y - (int) camera->geometry.y;
        render_texture(explosive->current_sprite, renderer, x, y,
                       (int) explosive->size.x,
                       (int) explosive->size.y);
}