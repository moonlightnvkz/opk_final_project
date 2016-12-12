//
// Created by moonlightnvkz on 11.12.16.
//

#include <assert.h>
#include "pointing_arrow.h"
#include "sdl_helpers.h"
#include "../default_values.h"
#include "../loggers.h"
#include "player.h"
#include "camera.h"

bool pa_create(PointingArrow *arrow, SDL_Renderer *renderer)
{
    assert(arrow != NULL);
    arrow->size.x = POINTING_ARROW_WIDTH;
    arrow->size.y = POINTING_ARROW_HEIGHT;
    arrow->texture = load_texture(POINTING_ARROW_TEXTURE, renderer);
    if (arrow->texture == NULL) {
        log_error("Failed to load texture", __FUNCTION__, __LINE__);
        arrow->texture = load_texture(MISSING_TEXTURE, renderer);
        if (arrow->texture == NULL) {
            log_error("Failed to load <missing_texture>", __FUNCTION__, __LINE__);
            return false;
        }
    }
    return true;
}

void pa_destroy(PointingArrow *arrow)
{
    SDL_DestroyTexture(arrow->texture);
}

static Vector2f get_relative_position(Vector2f point, Camera *camera)
{
    Vector2f rel_pos = {point.x - camera->geometry.x, point.y - camera->geometry.y};
    return rel_pos;
}

bool pa_need_to_render(Vector2f point_to, Camera *camera)
{
    Vector2f rel_pos = get_relative_position(point_to, camera);
    return rel_pos.x < 0 ||
           rel_pos.y < 0 ||
           rel_pos.x > camera->geometry.width ||
           rel_pos.y > camera->geometry.height;
}

void pa_render(PointingArrow *arrow, Camera *camera, SDL_Renderer *renderer, Vector2f point_from, Vector2f point_to)
{
    if (!pa_need_to_render(point_to, camera)) {
        return;
    }
    Vector2f rel_pos = get_relative_position(point_from, camera);
    double angle = atan2(point_to.y - point_from.y, point_to.x - point_from.x);
    int x = (int) (rel_pos.x + POINTING_ARROW_R * cos(angle) - arrow->size.x / 2.f);
    int y = (int) (rel_pos.y + POINTING_ARROW_R * sin(angle) - arrow->size.y / 2.f);
    render_texture_ex(arrow->texture, renderer, x, y, arrow->size.x, arrow->size.y, (int) rad_to_deg(angle) + 90);
}