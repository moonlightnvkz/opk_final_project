//
// Created by moonlightnvkz on 10.11.16.
//

#pragma once

#include "game_logic/geometry.h"

#define WINDOW_WIDTH                640
#define WINDOW_HEIGHT               480

#define MAP_WIDTH                   1000
#define MAP_HEIGHT                  1000

#define PLAYER_X                    400
#define PLAYER_Y                    400
#define PLAYER_WIDTH                50
#define PLAYER_HEIGHT               35
#define PLAYER_VELOCITY             600
#define PLAYER_FIRE_RATE            5   // Shots per second
#define PLAYER_COUNT                2

#define BULLET_VELOCITY             600
#define BULLET_MAX_AMOUNT           10  // For both players
#define BULLET_WIDTH                12
#define BULLET_HIEGHT               20
#define BULLET_TTL                  200

#define SERVER_IP                   "127.0.0.1"
#define SERVER_PORT                 8888
#define SERVER_TICKRATE             20
#define SERVER_START_SIGNAL         'S'

struct tGlobalVariables {
    ObjectGeometry map_geometry;
} GlobalVariables;

void globals_init();