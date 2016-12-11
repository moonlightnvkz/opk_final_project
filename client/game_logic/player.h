//
// Created by moonlightnvkz on 10.11.16.
//

#pragma once

#include <stdio.h>
#include <SDL_system.h>
#include "geometry.h"
#include "../default_values.h"

typedef struct tBullets Bullets;
typedef struct tResponseStructure ResponseStructure;
typedef struct tDeque Deque;
typedef struct tCamera Camera;

typedef struct tPlayer {
    ObjectGeometry geometry;
    SDL_Texture *texture;
    double angle;
    Vector2i velocity;
    bool shot_done;         // on this loop. After request is sent, shot_done -> false
    unsigned last_shot_time;
} Player;

struct tGlobalVariables {
    unsigned number_of_player;
} GlobalVariables;

Player *player_create(SDL_Renderer *renderer);

void player_destroy(Player *player);

void player_keystates_process(Player *player, const Uint8 *keystates);

void player_angle_process(Player *player, Camera *camera);

void player_move(Player *player, unsigned delta_ticks);

void player_render(Player *player, SDL_Renderer *renderer, Camera *camera);

void player_do_shot(Player *player, Bullets *bullets);

void player_apply_response_this(Player *player, Deque *requests_list, ResponseStructure *response);

void player_apply_response_others(Player *players[PLAYER_COUNT], ResponseStructure *response);