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

void camera_move_on(Camera *camera, float dx, float dy)
{
    camera->geometry.x += dx;
    camera->geometry.y += dy;
}

void camera_move_to(Camera *camera, float x, float y)
{
    camera->geometry.x = x;
    camera->geometry.y = y;
}

static float abs_float(float a) {
    return a > 0 ? a : -a;
}
bool camera_need_move_x(float dx)
{
    return abs_float(dx) > CAMERA_TRACK_DELAY_X;
}

bool camera_need_move_y(float dy)
{
    return abs_float(dy) > CAMERA_TRACK_DELAY_Y;
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
    if (camera_need_move_x(shift.x)) {
        shift.x > 0 ? shift.x -= (CAMERA_TRACK_DELAY_X) : (shift.x += CAMERA_TRACK_DELAY_X);
        camera->geometry.x += shift.x;
    }
    if (camera_need_move_y(shift.y)) {
        shift.y > 0 ? shift.y -= (CAMERA_TRACK_DELAY_Y) : (shift.y += CAMERA_TRACK_DELAY_Y);
        camera->geometry.y += shift.y;
    }
}