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

#ifndef M_PI
#define M_PI 3.14159265358979323846	/* pi */
#endif

bool bullets_create(Bullets *bullets, SDL_Renderer *renderer)
{
    bullets->number = 0;
    for (size_t i = 0; i < BULLET_MAX_AMOUNT; ++i) {
        bullets->bullets[i].geometry.x = bullets->bullets[i].geometry.y = 0;
        bullets->bullets[i].geometry.width = BULLET_WIDTH;
        bullets->bullets[i].geometry.height = BULLET_HIEGHT;
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
    *bullet1 = *bullet2;
    *bullet2 = temp;
}

static bool bullet_need_disactivate(Bullet *bullet) {
    return bullet->ttl <= 0;
}

static bool bullet_is_active(Bullet *bullet) {
    return bullet->ttl > 0;
}
void bullets_move_all(Bullets *bullets, unsigned delta_ticks)
{
    for (int i = 0; i < bullets->number; ++i) {
        Bullet *bullet = bullets->bullets + i;
        if (bullet_is_active(bullet))
        {
            bullet_move(bullet, delta_ticks);
            if (bullet_need_disactivate(bullet)) {
                bullets->number--;
                bullet_swap(bullet, bullets->bullets + bullets->number);
            }
        }
    }
}

static bool bullet_should_reflect(Bullet *bullet, unsigned delta_ticks)
{
    return false;
}

static void bullet_reflect(Bullet *bullet, unsigned delta_ticks)
{

}

void bullet_move(Bullet *bullet, unsigned delta_ticks)
{
    if (bullet_should_reflect(bullet, delta_ticks)) {
        bullet_reflect(bullet, delta_ticks);
    } else {
        float shift = (float) delta_ticks / 1000 * BULLET_VELOCITY;
        bullet->geometry.y -= shift * cos(bullet->angle / 180 * M_PI);
        bullet->geometry.x += shift * sin(bullet->angle / 180 * M_PI);
        bullet->ttl -= (int) shift;
    }
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

void bullets_apply_response(Bullets *bullets, BulletsStateResponse *response)
{
    bullets->number = response->number;
    for (unsigned i = 0; i < bullets->number; ++i) {
        Bullet *bullet = &bullets->bullets[i];
        BulletStateResponse *bulletStateResponse = &response->bullets[i];
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
    bullets->bullets[bullets->number].angle = angle;
    bullets->bullets[bullets->number].geometry.x = position.x;
    bullets->bullets[bullets->number].geometry.y = position.y;
    bullets->bullets[bullets->number].ttl = BULLET_TTL;
    bullets->number++;
    return true;
}