//
// Created by moonlightnvkz on 10.11.16.
//

#pragma once

#include "game_logic/geometry.h"

typedef struct tMapDescription MapDescription;
typedef struct tTileMap TileMap;

#define MAP_WIDTH                       1024    // must be a multiple of TILE_WIDTH
#define MAP_HEIGHT                      1024    // must be a multiple of TILE_HEIGHT
#define MAP_TILE_NUMBER                 2   // tiles stores in an array. Index - number of a tile.
                                            //Whole map stores in file like a 2-D array and loaded on start
#define MAP_LOAD_FILE                   "res/map/tilemap"

#define EXPLOSIVE_MAX_AMOUNT                    10
#define EXPLOSIVE_WIDTH                         64
#define EXPLOSIVE_HEIGHT                        64
#define EXPLOSIVE_DAMAGED_SPRITES_COUNT         2
#define EXPLOSIVE_EXPLOSION_SPRITES_COUNT       8
#define EXPLOSIVE_DAMAGED_ANIMATION_TIME        2000 //ms
#define EXPLOSION_DAMAGED_TICKS_IN_ANIMATION    20
#define EXPLOSIVE_EXPLOSION_ANIMATION_TIME      1000  //ms

#define TILE_WIDTH                      64
#define TILE_HEIGHT                     64

#define PLAYER_X                        400
#define PLAYER_Y                        400
#define PLAYER_WIDTH                    50
#define PLAYER_HEIGHT                   35
#define PLAYER_VELOCITY                 600
#define PLAYER_FIRE_RATE                5   // Shots per second
#define PLAYER_COUNT                    2
#define PLAYER_FIRE_START_DISTANCE      50

#define BULLET_VELOCITY                 600
#define BULLET_MAX_AMOUNT               10  // For both players
#define BULLET_WIDTH                    12
#define BULLET_HIEGHT                   20
#define BULLET_TTL                      200

#define SERVER_IP                       "127.0.0.1"
#define SERVER_PORT                     8888
#define SERVER_TICKRATE                 20
#define SERVER_START_SIGNAL             'S'

#define PLAYERS_ALIVE_TO_END_THE_GAME   0


struct tGlobalVariables {
    ObjectGeometry map_geometry;
    Vector2i tile_size;
    TileMap *map;
    MapDescription *map_descr;
} GlobalVariables;

void globals_init();