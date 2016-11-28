//
// Created by moonlightnvkz on 10.11.16.
//

#pragma once
#include <stdbool.h>

typedef struct tVector2i {
    int x;
    int y;
} Vector2i;

typedef struct tVector2f {
    float x;
    float y;
} Vector2f;

typedef struct tObjectGeometry
{
    float x;
    float y;
    int width;
    int height;
} ObjectGeometry;

typedef struct tIntRect {
    int x;
    int y;
    int width;
    int height;
} IntRect;

typedef struct tIntCircle {
    int x;
    int y;
    int radius;
} IntCircle;

bool geometry_rect_rect_intersects(IntRect rect1, IntRect rect2);

bool geometry_circle_rect_intersects(IntCircle circle, IntRect rect);