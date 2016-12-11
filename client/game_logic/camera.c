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

static bool camera_collision_check_x(Camera *camera, float dx)
{
    float new_x = camera->geometry.x + dx;
    return (new_x > 0 &&
            new_x + camera->geometry.width  < MAP_WIDTH);
}

static bool camera_collision_check_y(Camera *camera, float dy)
{
    float new_y = camera->geometry.y + dy;
    return (new_y > 0 &&
            new_y + camera->geometry.height  < MAP_HEIGHT);
}

void camera_move_on(Camera *camera, float dx, float dy)
{
    if (camera_collision_check_x(camera, dx)) {
        camera->geometry.x += dx;
    }
    if (camera_collision_check_y(camera, dy)) {
        camera->geometry.y += dy;
    }
}

bool camera_need_move_x(float dx)
{
    return fabs(dx) > CAMERA_TRACK_DELAY_X;
}

bool camera_need_move_y(float dy)
{
    return fabs(dy) > CAMERA_TRACK_DELAY_Y;
}

Vector2f camera_track_shift(Camera *camera, Player *player)
{
    Vector2f shift = {(float) (player->geometry.x - camera->geometry.x
                               - camera->geometry.width / 2.0  + player->geometry.width / 2.0),
                      (float) (player->geometry.y - camera->geometry.y
                               - camera->geometry.height / 2.0 + player->geometry.height / 2.0)};
    return shift;
}

void camera_move_after_the_player(Camera *camera, Player *player)
{
    Vector2f shift = camera_track_shift(camera, player);
    if (camera_need_move_x(shift.x)) {
        shift.x > 0 ? (shift.x -= CAMERA_TRACK_DELAY_X) : (shift.x += CAMERA_TRACK_DELAY_X);
    }
    if (camera_need_move_y(shift.y)) {
        shift.y > 0 ? (shift.y -= CAMERA_TRACK_DELAY_Y) : (shift.y += CAMERA_TRACK_DELAY_Y);
    }
    camera_move_on(camera, shift.x, shift.y);
}