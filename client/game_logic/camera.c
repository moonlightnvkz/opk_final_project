//
// Created by moonlightnvkz on 04.12.16.
//

#include <SDL_mouse.h>
#include "camera.h"
#include "../default_values.h"
#include "player.h"

bool camera_create(Camera *camera, SDL_Renderer *renderer)
{
    if (!pa_create(&camera->arrow, renderer)) {
        return NULL;
    }
    camera->geometry.x = CAMERA_START_X;
    camera->geometry.y = CAMERA_START_Y;
    camera->geometry.width = CAMERA_WIDTH;
    camera->geometry.height = CAMERA_HEIGHT;
    return true;
}

void camera_destroy(Camera *camera)
{
    pa_destroy(&camera->arrow);
}

void camera_move_on(Camera *camera, float dx, float dy)
{
    ObjectGeometry new_geom = {camera->geometry.x + dx,
                               camera->geometry.y,
                               camera->geometry.width,
                               camera->geometry.height};
    if (geometry_rect_rect_collision_check(new_geom, true, GlobalVariables.map_geometry)) {
        camera->geometry.x += dx;
    }
    new_geom.x -= dx;
    new_geom.y += dy;
    if (geometry_rect_rect_collision_check(new_geom, true, GlobalVariables.map_geometry)) {
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

static int signum(double a)
{
    return a > 0.001 ? 1 : (a < -0.001 ? -1 : 0);
}

void camera_move_after_the_mouse(Camera *camera, Player *player)
{
    Vector2i mouse_pos;
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

    double s = sin(deg_to_rad(player->angle));
    double c = cos(deg_to_rad(player->angle));
    Vector2f player_rel_pos = player_get_relative_position(player, camera);

    Vector2f d_pos = { (mouse_pos.x + player_rel_pos.x - camera->geometry.width ) / 2.f,
                       (mouse_pos.y + player_rel_pos.y - camera->geometry.height) / 2.f};
    camera_move_on(camera, d_pos.x, d_pos.y);
}

void camera_render_pointing_arrow(Camera *camera, SDL_Renderer *renderer, Vector2f point_from, Vector2f point_to)
{
    pa_render(&camera->arrow, camera, renderer, point_from, point_to);
}