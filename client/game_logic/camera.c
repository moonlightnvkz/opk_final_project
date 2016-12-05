//
// Created by moonlightnvkz on 04.12.16.
//

#include <stdlib.h>
#include "camera.h"
#include "../default_values.h"
#include "player.h"

Camera *camera_create()
{
    Camera *c = malloc(sizeof(Camera));
    if (!c) {
        return NULL;
    }
    c->geometry.x = CAMERA_START_X;
    c->geometry.y = CAMERA_START_Y;
    c->geometry.width = CAMERA_WIDTH;
    c->geometry.height = CAMERA_HEIGHT;
    return c;
}

void camera_destroy(Camera *camera)
{
    free(camera);
}

static bool camera_collision_check(Camera *camera, float dx, float dy)
{
    Vector2f new_pos = {camera->geometry.x + dx, camera->geometry.y + dy};
    return (new_pos.x > 0 &&
            new_pos.y > 0 &&
            new_pos.x + camera->geometry.width  < MAP_WIDTH &&
            new_pos.y + camera->geometry.height < MAP_HEIGHT);
}

void camera_move_on(Camera *camera, float dx, float dy)
{
    if (camera_collision_check(camera, dx, dy)) {
        camera->geometry.x += dx;
        camera->geometry.y += dy;
    }
}

void camera_move_to(Camera *camera, float x, float y)
{
    camera->geometry.x = x;
    camera->geometry.y = y;
}

bool camera_need_move_x(float dx)
{
    return true;
    return fabs(dx) > CAMERA_TRACK_DELAY_X;
}

bool camera_need_move_y(float dy)
{
    return true;
    return fabs(dy) > CAMERA_TRACK_DELAY_Y;
}

Vector2f camera_track_shift(Camera *camera, Player *player)
{
    Vector2f shift = {(float) (player->geometry.x - camera->geometry.x -
                               camera->geometry.width / 2.0  + player->geometry.width / 2.0),
                      (float) (player->geometry.y - camera->geometry.y -
                               camera->geometry.height / 2.0 + player->geometry.height / 2.0)};
    return shift;
}

void camera_move_after_the_player(Camera *camera, Player *player)
{
    Vector2f shift = camera_track_shift(camera, player);
    if (camera_need_move_x(shift.x) && camera_collision_check(camera, shift.x, 0.f)) {
        shift.x > 0 ? shift.x -= (CAMERA_TRACK_DELAY_X) : (shift.x += CAMERA_TRACK_DELAY_X);
        camera->geometry.x += shift.x;
    }
    if (camera_need_move_y(shift.y) && camera_collision_check(camera, 0.f, shift.y)) {
        shift.y > 0 ? shift.y -= (CAMERA_TRACK_DELAY_Y) : (shift.y += CAMERA_TRACK_DELAY_Y);
        camera->geometry.y += shift.y;
    }
}