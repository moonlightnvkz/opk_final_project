//
// Created by moonlightnvkz on 10.11.16.
//

#pragma once

#include "game_logic/geometry.h"

typedef struct tMapDescription MapDescription;
typedef struct tTileMap TileMap;

#define WINDOW_WIDTH                            640
#define WINDOW_HEIGHT                           480

#define TEXT_FONT_PATH                          "res/OpenSans-Regular.ttf"
#define TEXT_WAITING_FOR_PLAYERS                "Waiting for other players..."
#define TEXT_FONT_SIZE                          48

#define CAMERA_START_X                          1
#define CAMERA_START_Y                          1
#define CAMERA_WIDTH                            WINDOW_WIDTH
#define CAMERA_HEIGHT                           WINDOW_HEIGHT
#define CAMERA_TRACK_DELAY_X                    200
#define CAMERA_TRACK_DELAY_Y                    200

#define EXPLOSIVE_MAX_AMOUNT                    10
#define EXPLOSIVE_WIDTH                         64
#define EXPLOSIVE_HEIGHT                        64
#define EXPLOSIVE_DAMAGED_SPRITES_COUNT         2
#define EXPLOSIVE_EXPLOSION_SPRITES_COUNT       8
#define EXPLOSIVE_DAMAGED_ANIMATION_TIME        2000 //ms
#define EXPLOSION_DAMAGED_TICKS_IN_ANIMATION    20
#define EXPLOSIVE_EXPLOSION_ANIMATION_TIME      1000  //ms
#define EXPLOSIVE_EXPLOSION_TEXTURE             "res/explosion/0.png"
#define EXPLOSIVE_EXPLOSION_NUMBER_IDX          14
#define EXPLOSIVE_DAMAGED_TEXTURE               "res/explosion/bomb0.png"
#define EXPLOSIVE_DAMAGED_NUMBER_IDX            18

#define POINTING_ARROW_R                        200
#define POINTING_ARROW_WIDTH                    40
#define POINTING_ARROW_HEIGHT                   120

#define MAP_WIDTH                               1024    // must be a multiple of TILE_WIDTH
#define MAP_HEIGHT                              1024    // must be a multiple of TILE_HEIGHT
#define MAP_TILE_NUMBER                         2   // tiles stores in an array. Index - number of a tile.
                                                    // Whole map stores in file like a 2-D array and loaded on start

#define TILE_WIDTH                              64
#define TILE_HEIGHT                             64

#define PLAYER_START_X                          200
#define PLAYER_START_Y                          200
#define PLAYER_WIDTH                            50
#define PLAYER_HEIGHT                           35
#define PLAYER_VELOCITY                         400
#define PLAYER_FIRE_RATE                        5   // Shots per second
#define PLAYER_COUNT                            2
#define PLAYER_FIRE_START_DISTANCE              50

#define BULLET_VELOCITY                         600
#define BULLET_MAX_AMOUNT                       10
#define BULLET_WIDTH                            12
#define BULLET_HEIGHT                           20
#define BULLET_TTL                              400 // distance

#define PLAYER_TEXTURE                          "res/player/player_texture.png"
#define BULLET_TEXTURE                          "res/bullets/bullet_texture.png"
#define POINTING_ARROW_TEXTURE                  "res/arrow/pointing_arrow_texture.png"
#define MISSING_TEXTURE                         "res/missing_texture.png"
#define TILE_TEXTURE                            "res/map/0.png" // 0 will be changed to other numbers,
#define TILE_TEXTURE_NUMBER_IDX                 8               // so max number of tiles mustn't exceed 9
#define MAP_LOAD_FILE                           "res/map/tilemap"

#define SERVER_IP                               "192.168.1.34"
#define SERVER_PORT                             8888
#define SERVER_TICKRATE                         20
#define SERVER_START_SIGNAL                     'S'


struct tGlobalVariables {
    bool quit;
    unsigned number_of_player;
    ObjectGeometry map_geometry;
    Vector2i tile_size;
    TileMap *map;
    MapDescription *map_descr;
} GlobalVariables;

void globals_init();