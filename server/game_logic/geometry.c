//
// Created by moonlightnvkz on 10.11.16.
//
#include <stdlib.h> //abs()
#include <math.h>
#include "geometry.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846	/* pi */
#endif

bool geometry_rect_rect_collision_check(ObjectGeometry rect1, bool is_rect1_inside_rect2, ObjectGeometry rect2)
{
    if (is_rect1_inside_rect2) {
        return rect1.x > rect2.x &&
               rect1.y > rect2.y &&
               rect1.x + rect1.width  < rect2.x + rect2.width &&
               rect1.y + rect1.height < rect2.y + rect2.height;
    } else {
        return rect1.x + rect1.width  < rect2.x ||
               rect2.x + rect2.width  < rect1.x ||
               rect1.y + rect1.height < rect2.y ||
               rect2.y + rect2.height < rect1.y;
    }
}

double rad_to_deg(double rad)
{
    return rad / M_PI * 180;
}

double deg_to_rad(double deg)
{
    return deg / 180 * M_PI;
}