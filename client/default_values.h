//
// Created by moonlightnvkz on 10.11.16.
//

#pragma once

#define WINDOW_WIDTH                640
#define WINDOW_HEIGHT               480

#define CAMERA_START_X              1
#define CAMERA_START_Y              1
#define CAMERA_WIDTH                WINDOW_WIDTH
#define CAMERA_HEIGHT               WINDOW_HEIGHT
#define CAMERA_TRACK_DELAY_X        200
#define CAMERA_TRACK_DELAY_Y        200

#define MAP_WIDTH                   1000
#define MAP_HEIGHT                  1000

#define TILE_WIDTH                  64
#define TILE_HEIGHT                 64

#define PLAYER_X                    400
#define PLAYER_Y                    400
#define PLAYER_WIDTH                50
#define PLAYER_HEIGHT               35
#define PLAYER_VELOCITY             400
#define PLAYER_FIRE_RATE            5   // Shots per second
#define PLAYER_COUNT                2

#define BULLET_VELOCITY             600
#define BULLET_MAX_AMOUNT           40
#define BULLET_WIDTH                12
#define BULLET_HIEGHT               20
#define BULLET_TTL                  400 // distance

#define TILE_TEXTURE                "res/floor_texture.png"
#define PLAYER_TEXTURE              "res/player_texture.png"
#define BULLET_TEXTURE              "res/bullet_texture.png"
#define MISSING_TEXTURE             "res/missing_texture.png"

#define SERVER_IP                   "127.0.0.1"
#define SERVER_PORT                 8888
#define SERVER_TICKRATE             20
#define SERVER_START_SIGNAL         'S'