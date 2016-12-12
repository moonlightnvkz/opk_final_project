//
// Created by moonlightnvkz on 10.11.16.
//

#pragma once

#include <stdio.h>
#include <SDL_system.h>
#include "geometry.h"

typedef struct tBullets Bullets;
typedef struct tRequestStructure RequestStructure;


typedef struct tPlayer {
    bool is_alive;
    ObjectGeometry geometry;
    int angle;
    Vector2i velocity;
    bool shot_done;
    unsigned last_request_time;
    unsigned last_shot_time;
} Player;


bool player_create(Player *player);

void player_destroy(Player *player);

void player_move(Player *player, unsigned delta_ticks);

void player_do_shot(Player *player, Bullets *bullets);

void player_apply_request(Player *player, RequestStructure *request);