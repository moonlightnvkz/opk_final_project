//
// Created by moonlightnvkz on 10.11.16.
//

#pragma once

#include "game_logic/geometry.h"

typedef struct tMapDescription MapDescription;

#define WINDOW_WIDTH                    640
#define WINDOW_HEIGHT                   480

#define CAMERA_START_X                  1
#define CAMERA_START_Y                  1
#define CAMERA_WIDTH                    WINDOW_WIDTH
#define CAMERA_HEIGHT                   WINDOW_HEIGHT
#define CAMERA_TRACK_DELAY_X            200
#define CAMERA_TRACK_DELAY_Y            200

#define POINTING_ARROW_R                200
#define POINTING_ARROW_WIDTH            40
#define POINTING_ARROW_HEIGHT           120

#define MAP_WIDTH                       1024    // must be a multiple of TILE_WIDTH
#define MAP_HEIGHT                      1024    // must be a multiple of TILE_HEIGHT
#define MAP_TILE_NUMBER                 2   // tiles stores in an array. Index - number of a tile.
                                            //Whole map stores in file like a 2-D array and loaded on start

#define TILE_WIDTH                      64
#define TILE_HEIGHT                     64

#define PLAYER_START_X                  200
#define PLAYER_START_Y                  200
#define PLAYER_WIDTH                    50
#define PLAYER_HEIGHT                   35
#define PLAYER_VELOCITY                 400
#define PLAYER_FIRE_RATE                5   // Shots per second
#define PLAYER_COUNT                    2
#define PLAYER_FIRE_START_DISTANCE      50

#define BULLET_VELOCITY                 600
#define BULLET_MAX_AMOUNT               10
#define BULLET_WIDTH                    12
#define BULLET_HEIGHT                   20
#define BULLET_TTL                      200 // distance

#define PLAYER_TEXTURE                  "res/player_texture.png"
#define BULLET_TEXTURE                  "res/bullet_texture.png"
#define POINTING_ARROW_TEXTURE          "res/pointing_arrow_texture.png"
#define MISSING_TEXTURE                 "res/missing_texture.png"
#define TILE_TEXTURE                    "res/map/0.png" // 0 will be changed to other numbers,
#define TILE_TEXTURE_NUMBER_IDX         8               // so max number of tiles mustn't exceed 9
#define MAP_LOAD_FILE                   "res/map/tilemap"

#define SERVER_IP                       "127.0.0.1"
#define SERVER_PORT                     8888
#define SERVER_TICKRATE                 20
#define SERVER_START_SIGNAL             'S'


struct tGlobalVariables {
    bool quit;
    unsigned number_of_player;
    ObjectGeometry map_geometry;
    Vector2i tile_size;
    MapDescription *map_descr;
} GlobalVariables;

void globals_init();