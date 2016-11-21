//
// Created by moonlightnvkz on 10.11.16.
//
#include <stdlib.h> //abs()
#include <math.h>
#include "geometry.h"

bool Rect_is_intersects(IntRect rect1, IntRect rect2)
{
    return !(rect1.x + rect1.width  < rect2.x ||
             rect1.y + rect1.height < rect2.y ||
             rect2.x + rect2.width  < rect1.x ||
             rect2.y + rect2.height < rect2.y);
}

bool Circle_Rect_intersects(IntCircle circle, IntRect rect)
{
    Vector2i circleDistance;
    circleDistance.x = abs(circle.x - rect.x);
    circleDistance.y = abs(circle.y - rect.y);

    if (circleDistance.x > (rect.width/2  + circle.radius) ||
        circleDistance.y > (rect.height/2 + circle.radius)) {
        return false;
    }

    if (circleDistance.x <= (rect.width/2 ) ||
        circleDistance.y <= (rect.height/2)) {
        return true;
    }

    float cornerDistance_sq = (circleDistance.x - rect.width /2)^2 +
                              (circleDistance.y - rect.height/2)^2;

    return (cornerDistance_sq <= (circle.radius^2));
}
