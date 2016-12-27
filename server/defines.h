//
// Created by moonlightnvkz on 10.11.16.
//

#pragma once

#define MAP_WIDTH                               1024    // must be a multiple of TILE_WIDTH
#define MAP_HEIGHT                              1024    // must be a multiple of TILE_HEIGHT
#define MAP_LOAD_FILE                           "res/map/tilemap"

#define EXPLOSIVE_MAX_AMOUNT                    20
#define EXPLOSIVE_WIDTH                         64
#define EXPLOSIVE_HEIGHT                        64
#define EXPLOSIVE_DAMAGED_ANIMATION_TIME        2000    //ms
#define EXPLOSIVE_EXPLOSION_ANIMATION_TIME      1000    //ms

#define TILE_WIDTH                              64
#define TILE_HEIGHT                             64

#define PLAYER_START_X                          200
#define PLAYER_START_Y                          200
#define PLAYER_WIDTH                            50
#define PLAYER_HEIGHT                           35
#define PLAYER_VELOCITY                         500
#define PLAYER_FIRE_RATE                        5   // Shots per second
#define PLAYER_COUNT                            2
#define PLAYER_FIRE_START_DISTANCE              50

#define BULLET_VELOCITY                         800
#define BULLET_MAX_AMOUNT                       10  // For both players
#define BULLET_WIDTH                            12
#define BULLET_HIEGHT                           20
#define BULLET_TTL                              400

#define SERVER_TICKRATE                         20
#define SERVER_START_SIGNAL                     'S'
#define SERVER_CONF_FILE                        "conf/server"

#define GAME_CONF_FILE                          "conf/game"