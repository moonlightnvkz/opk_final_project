//
// Created by moonlightnvkz on 20.11.16.
//

#pragma once

typedef struct tRequestStructure RequestStructure;
typedef struct tResponseStructure ResponseStructure;
typedef struct tPlayer Player;
typedef struct tBullets Bullets;

typedef struct tModelController {
    Player *player1;
    Player *player2;
    Bullets *bullets;
} ModelController;

ModelController *mc_init();

void mc_destroy(ModelController *mc);

void mc_process_moving(ModelController *mc, unsigned delta_ticks);

void mc_apply_request(ModelController *mc, Player *player, RequestStructure *request);

void mc_create_response(ModelController *mc, int player, unsigned req_number, ResponseStructure *res);

void mc_reset_temp_flags(ModelController *mc);