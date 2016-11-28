//
// Created by moonlightnvkz on 10.11.16.
//

#pragma once

#define WINDOW_WIDTH                640
#define WINDOW_HEIGHT               480

#define PLAYER_X                    WINDOW_WIDTH / 10
#define PLAYER_Y                    WINDOW_HEIGHT / 10
#define PLAYER_WIDTH                50
#define PLAYER_HEIGHT               35
#define PLAYER_VELOCITY             600
#define PLAYER_FIRE_RATE            5   // Shots per second

#define BULLET_VELOCITY             600
#define BULLET_MAX_AMOUNT           40  // For both players
#define BULLET_WIDTH                12
#define BULLET_HIEGHT               20

#define PLAYER_TEXTURE              "res/player_texture.png"
#define BULLET_TEXTURE              "res/bullet_texture.png"
#define MISSING_TEXTURE             "res/missing_texture.png"

#define SERVER_IP                   "127.0.0.1"
#define SERVER_PORT                 8888
#define SERVER_TICKRATE             20
#define SERVER_START_SIGNAL         'S'
#define SERVER_START_SIGNAL_LENGTH  1