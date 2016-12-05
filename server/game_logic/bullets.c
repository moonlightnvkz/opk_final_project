//
// Created by moonlightnvkz on 15.11.16.
//

#include <stdlib.h>
#include <math.h>
#include "bullets.h"
#include "../default_values.h"
#include "../loggers.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846	/* pi */
#endif

Bullets *bullets_create()
{
    Bullets *bullets = malloc(sizeof(Bullets));
    if (bullets == NULL) {
        log_error("Failed to allocate memory for bullets struct", __FUNCTION__, __LINE__);
        return NULL;
    }

    bullets->number = 0;
    for (size_t i = 0; i < BULLET_MAX_AMOUNT; ++i) {
        bullets->bullets[i].geometry.x = bullets->bullets[i].geometry.y = 0;
        bullets->bullets[i].geometry.width = BULLET_WIDTH;
        bullets->bullets[i].geometry.height = BULLET_HIEGHT;
        bullets->bullets[i].angle = 0;
        bullets->bullets[i].ttl = 0;
    }
    return bullets;
}

void bullets_destroy(Bullets *bullets)
{
    free(bullets);
}

static void bullet_swap(Bullet *bullet1, Bullet *bullet2)
{
    Bullet temp = *bullet1;
    *bullet1 = *bullet2;
    *bullet2 = temp;
}

static bool bullet_need_disactivate(Bullet *bullet) {
    return bullet->ttl < 0;
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
        bullet->ttl -= shift;
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