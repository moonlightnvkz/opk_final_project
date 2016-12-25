//
// Created by moonlightnvkz on 15.11.16.
//

#include <stdlib.h>
#include <math.h>
#include "bullets.h"
#include "../loggers.h"
#include "sdl_helpers.h"
#include "../server_logic/request_response.h"
#include "camera.h"
#include "player.h"
#include "tile_map.h"
#include "../globals.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846	/* pi */
#endif

void bullet_move(Bullet *bullet, unsigned delta_ticks, TileMap *map, Player players[PLAYER_COUNT]);

bool bullets_create(Bullets *bullets, SDL_Renderer *renderer)
{
    bullets->number = 0;
    for (size_t i = 0; i < BULLET_MAX_AMOUNT; ++i) {
        bullets->bullets[i].geometry.x = bullets->bullets[i].geometry.y = 0;
        bullets->bullets[i].geometry.width = BULLET_WIDTH;
        bullets->bullets[i].geometry.height = BULLET_HEIGHT;
        bullets->bullets[i].angle = 0;
        bullets->bullets[i].ttl = 0;
    }

    bullets->texture = load_texture(BULLET_TEXTURE, renderer);
    if (bullets->texture == NULL) {
        LOG_ACTION(Failed to load texture);
        bullets->texture = load_texture(MISSING_TEXTURE, renderer);
        if (bullets->texture == NULL) {
            LOG_ACTION(Failed to load <missing_texture>);
            return NULL;
        }
    }
    return true;
}

void bullets_destroy(Bullets *bullets)
{
    SDL_DestroyTexture(bullets->texture);
}

static void bullet_swap(Bullet *bullet1, Bullet *bullet2)
{
    Bullet temp = *bullet1;
    memcpy(bullet1, bullet2, sizeof(Bullet));
    memcpy(bullet2, &temp, sizeof(Bullet));
}

static bool bullet_need_disactivate(Bullet *bullet) {
    return bullet->ttl <= 0;
}

static bool bullet_is_active(Bullet *bullet) {
    return bullet->ttl > 0;
}
void bullets_move_all(Bullets *bullets, unsigned delta_ticks, TileMap *map, Player players[PLAYER_COUNT])
{
    for (int i = 0; i < bullets->number; ++i) {
        Bullet *bullet = bullets->bullets + i;
        if (bullet_need_disactivate(bullet)) {
            bullets->number--;
            bullet_swap(bullet, bullets->bullets + bullets->number);
            continue;
        }
        if (bullet_is_active(bullet))
        {
            bullet_move(bullet, delta_ticks, map, players);
        }
    }
}

static void bullet_reflect(Bullet *bullet, TileMap *map)
{
    double x = bullet->geometry.x;
    double y = bullet->geometry.y;
    double w = bullet->geometry.width;
    double h = bullet->geometry.height;

    bool x_reflect = false, y_reflect = false;
    if (x < map->geometry.x) {
        bullet->geometry.x = 2 * map->geometry.x - bullet->geometry.x;
        y_reflect = true;
    }
    if (y < map->geometry.y) {
        bullet->geometry.y = 2 * map->geometry.y - bullet->geometry.y;
        x_reflect = true;
    }
    if (x + w > map->geometry.x + map->geometry.width) {
        bullet->geometry.x -= 2 * (x + w - map->geometry.x - map->geometry.width);
        y_reflect = true;
    }
    if (y + h > map->geometry.y + map->geometry.height) {
        bullet->geometry.y -= 2 * (y + h - map->geometry.y - map->geometry.height);
        x_reflect = true;
    }

    if (x_reflect) {
        bullet->angle = 180 - bullet->angle;
    }
    if (y_reflect) {
        bullet->angle = -bullet->angle;
    }
}

void bullet_collision(Bullet *bullet, TileMap *map, Player players[PLAYER_COUNT])
{
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        if (players[i].is_alive && !geometry_rect_rect_collision_check(bullet->geometry, false, players[i].geometry)) {
            player_kill(&players[i]);
            bullet->ttl = 0;
        }
    }
    if (!tilemap_collision_check(map, bullet->geometry)) {
        bullet->ttl = 0;
    }
}

void bullet_move(Bullet *bullet, unsigned delta_ticks, TileMap *map, Player players[PLAYER_COUNT])
{
    float shift = (float) delta_ticks / 1000 * BULLET_VELOCITY;
    bullet->geometry.x += shift * sin(deg_to_rad(bullet->angle));
    bullet->geometry.y -= shift * cos(deg_to_rad(bullet->angle));
    bullet->ttl -= shift;

    bullet_collision(bullet, map, players);
    bullet_reflect(bullet, map);
}

void bullets_render_all(Bullets *bullets, SDL_Renderer *renderer, Camera *camera)
{
    for (int i = 0; i < bullets->number; ++i) {
        Bullet *bullet = bullets->bullets + i;
        if (bullet_is_active(bullet))
        {
            bullet_render(bullet, bullets->texture, renderer, camera);
        }
    }
}

Vector2f bullet_get_relative_position(Bullet *bullet, Camera *camera)
{
    Vector2f rel_pos = {bullet->geometry.x - camera->geometry.x, bullet->geometry.y - camera->geometry.y};
    return rel_pos;
}

void bullet_render(Bullet *bullet, SDL_Texture *texture, SDL_Renderer *renderer, Camera *camera)
{
    Vector2f rel_pos = bullet_get_relative_position(bullet, camera);
    render_texture_ex(texture, renderer,
                      (int)rel_pos.x,
                      (int)rel_pos.y,
                      bullet->geometry.width,
                      bullet->geometry.height,
                      bullet->angle);
}

void bullets_apply_response(Bullets *bullets, BulletsStateResponse *state)
{
    bullets->number = state->number;
    for (unsigned i = 0; i < bullets->number; ++i) {
        Bullet *bullet = bullets->bullets + i;
        BulletStateResponse *bulletStateResponse = state->bullets + i;
        bullet->ttl = bulletStateResponse->ttl;
        bullet->angle = bulletStateResponse->angle;
        bullet->geometry.x = bulletStateResponse->position.x;
        bullet->geometry.y = bulletStateResponse->position.y;
    }
}

bool bullets_add_bullet(Bullets *bullets, Vector2f position, double angle)
{
    if (bullets->number == BULLET_MAX_AMOUNT) {
        return false;
    }
    Bullet *bullet = &bullets->bullets[bullets->number];
    bullet->angle = angle;
    bullet->geometry.x = position.x - bullet->geometry.width / 2.f;
    bullet->geometry.y = position.y - bullet->geometry.height / 2.f;
    bullet->ttl = BULLET_TTL;
    bullets->number++;
    return true;
}