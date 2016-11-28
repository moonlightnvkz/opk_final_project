//
// Created by moonlightnvkz on 15.11.16.
//

#include <stdlib.h>
#include <math.h>
#include "bullets.h"
#include "../default_values.h"
#include "../loggers.h"
#include "sdl_helpers.h"
#include "../server_logic/request_response.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846	/* pi */
#endif

Bullets *bullets_create(SDL_Renderer *renderer)
{
    Bullets *bullets = malloc(sizeof(Bullets));
    if (bullets == NULL) {
        log_error("Failed to allocate memory for bullets struct", __FUNCTION__, __LINE__);
        return NULL;
    }
    bullets->bullets = malloc(sizeof(Bullet) * BULLET_MAX_AMOUNT);
    if (bullets->bullets == NULL) {
        log_error("Failed to allocate memory for bullets array", __FUNCTION__, __LINE__);
        free(bullets);
        return NULL;
    }

    bullets->number = 0;
    for (size_t i = 0; i < BULLET_MAX_AMOUNT; ++i) {
        bullets->bullets[i].geometry.x = bullets->bullets[i].geometry.y = 0;
        bullets->bullets[i].geometry.width = BULLET_WIDTH;
        bullets->bullets[i].geometry.height = BULLET_HIEGHT;
        bullets->bullets[i].angle = 0;
        bullets->bullets[i].active = false;
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
    return bullets;
}

void bullets_destroy(Bullets *bullets)
{
    SDL_DestroyTexture(bullets->texture);
    free(bullets->bullets);
    free(bullets);
}

static void bullet_swap(Bullet *bullet1, Bullet *bullet2)
{
    Bullet temp = *bullet1;
    *bullet1 = *bullet2;
    *bullet2 = temp;
}

static bool bullet_need_disactivate(Bullet *bullet) {
    if (bullet->geometry.x < 0 || bullet->geometry.x > WINDOW_WIDTH ||
        bullet->geometry.y < 0 || bullet->geometry.y > WINDOW_HEIGHT)
    {
        return true;
    } else {
        return false;
    }
}

void bullets_move_all(Bullets *bullets, unsigned delta_ticks)
{
    for (int i = 0; i < bullets->number; ++i) {
        Bullet *bullet = bullets->bullets + i;
        if (bullet->active == true)
        {
            bullet_move(bullet, delta_ticks);
            if (bullet_need_disactivate(bullet)) {
                bullet->active = false;
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
        bullet->geometry.y -= (float) delta_ticks / 1000 * BULLET_VELOCITY * cos(bullet->angle / 180 * M_PI);
        bullet->geometry.x += (float) delta_ticks / 1000 * BULLET_VELOCITY * sin(bullet->angle / 180 * M_PI);
    }
}

void bullets_render_all(Bullets *bullets, SDL_Renderer *renderer)
{
    for (int i = 0; i < bullets->number; ++i) {
        Bullet *bullet = bullets->bullets + i;
        if (bullet->active == true)
        {
            bullet_render(bullet, bullets->texture, renderer);
        }
    }
}

void bullet_render(Bullet *bullet, SDL_Texture *texture, SDL_Renderer *renderer)
{
    render_texture_ex(texture, renderer,
                      (int)bullet->geometry.x,
                      (int)bullet->geometry.y,
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
        bullet->active = bulletStateResponse->active;
        bullet->angle = bulletStateResponse->angle;
        bullet->geometry.x = bulletStateResponse->position.x;
        bullet->geometry.y = bulletStateResponse->position.y;
    }
}