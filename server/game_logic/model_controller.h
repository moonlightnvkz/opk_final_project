//
// Created by moonlightnvkz on 20.11.16.
//

#pragma once

#include "../defines.h"
#include "bullets.h"
#include "player.h"
#include "tile_map.h"

typedef struct tRequestStructure RequestStructure;
typedef struct tResponseStructure ResponseStructure;

typedef struct tModelController {
    Player players[PLAYER_COUNT];
    Bullets bullets;
    TileMap map;
} ModelController;

bool mc_init(ModelController *mc);

void mc_destroy(ModelController *mc);

void mc_process_moving(ModelController *mc, unsigned delta_ticks);

int mc_alive_players_count(ModelController *mc);

void mc_apply_request(ModelController *mc, unsigned number_of_player, RequestStructure *request);