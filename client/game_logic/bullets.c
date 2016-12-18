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

#ifndef M_PI
#define M_PI 3.14159265358979323846	/* pi */
#endif

void bullet_move(Bullet *bullet, unsigned delta_ticks, Player players[PLAYER_COUNT]);

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
        log_error("Failed to load texture", __FUNCTION__, __LINE__);
        bullets->texture = load_texture(MISSING_TEXTURE, renderer);
        if (bullets->texture == NULL) {
            log_error("Failed to load <missing_texture>", __FUNCTION__, __LINE__);
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
    bullet1->angle = bullet2->angle;
    bullet1->ttl = bullet2->ttl;
    bullet1->geometry.x = bullet2->geometry.x;
    bullet1->geometry.y = bullet2->geometry.y;
    bullet1->geometry.width = bullet2->geometry.width;
    bullet1->geometry.height = bullet2->geometry.height;

    bullet2->angle = temp.angle;
    bullet2->ttl = temp.ttl;
    bullet2->geometry.x = temp.geometry.x;
    bullet2->geometry.y = temp.geometry.y;
    bullet2->geometry.width = temp.geometry.width;
    bullet2->geometry.height = temp.geometry.height;
}

static bool bullet_need_disactivate(Bullet *bullet) {
    return bullet->ttl <= 0;
}

static bool bullet_is_active(Bullet *bullet) {
    return bullet->ttl > 0;
}
void bullets_move_all(Bullets *bullets, unsigned delta_ticks, Player players[PLAYER_COUNT])
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
            bullet_move(bullet, delta_ticks, players);
        }
    }
}

static void bullet_reflect(Bullet *bullet)
{
    double x = bullet->geometry.x;
    double y = bullet->geometry.y;
    double w = bullet->geometry.width;
    double h = bullet->geometry.height;

    bool x_reflect = false, y_reflect = false;
    if (x < GlobalVariables.map_geometry.x) {
        bullet->geometry.x = 2 * GlobalVariables.map_geometry.x - bullet->geometry.x;
        y_reflect = true;
    }
    if (y < GlobalVariables.map_geometry.y) {
        bullet->geometry.y = 2 * GlobalVariables.map_geometry.y - bullet->geometry.y;
        x_reflect = true;
    }
    if (x + w > GlobalVariables.map_geometry.x + GlobalVariables.map_geometry.width) {
        bullet->geometry.x -= 2 * (x + w - GlobalVariables.map_geometry.x - GlobalVariables.map_geometry.width);
        y_reflect = true;
    }
    if (y + h > GlobalVariables.map_geometry.y + GlobalVariables.map_geometry.height) {
        bullet->geometry.y -= 2 * (y + h - GlobalVariables.map_geometry.y - GlobalVariables.map_geometry.height);
        x_reflect = true;
    }

    if (x_reflect) {
        bullet->angle = 180 - bullet->angle;
    }
    if (y_reflect) {
        bullet->angle = -bullet->angle;
    }
}

void bullet_collision(Bullet *bullet, Player players[PLAYER_COUNT]/*, Object *objects,*/)
{
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        if (players[i].is_alive && !geometry_rect_rect_collision_check(bullet->geometry, false, players[i].geometry)) {
            player_kill(&players[i]);
            bullet->ttl = 0;
        }
    }
}

void bullet_move(Bullet *bullet, unsigned delta_ticks, Player players[PLAYER_COUNT])
{
    float shift = (float) delta_ticks / 1000 * BULLET_VELOCITY;
    bullet->geometry.x += shift * sin(deg_to_rad(bullet->angle));
    bullet->geometry.y -= shift * cos(deg_to_rad(bullet->angle));
    bullet->ttl -= shift;

    bullet_collision(bullet, players/*, objects,*/);
    bullet_reflect(bullet);
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