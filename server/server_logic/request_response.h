//
// Created by moonlightnvkz on 19.11.16.
//

#pragma once

#include <stdbool.h>
#include "../game_logic/geometry.h"
#include "../default_values.h"

typedef struct tPlayer Player;
typedef struct tBullets Bullets;


#pragma pack(push, 1)
typedef struct tPlayerStateRequest {
    double angle;
    Vector2f position;
    Vector2i velocity;
    bool shot_done;
} PlayerStateRequest;

typedef struct tRequestStructure {
    unsigned req_number;
    PlayerStateRequest player_state;
} RequestStructure;


typedef struct tPlayerStateResponse {
    double angle;
    Vector2f position;
    Vector2i velocity;
} PlayerStateResponse;

typedef struct tBulletStateResponse {
    Vector2f position;
    double angle;
    int ttl;
} BulletStateResponse;

typedef struct tBulletsStateResponse {
    BulletStateResponse bullets[BULLET_MAX_AMOUNT];
    unsigned number;
} BulletsStateResponse;

typedef struct tResponseStructure {
    unsigned res_number;
    PlayerStateResponse this_player_state;
    PlayerStateResponse diff_player_state;
    BulletsStateResponse bullets;
} ResponseStructure;
#pragma pack(pop)

void response_set_player_states(PlayerStateResponse *state, Player *player);

void response_set_bullets_states(BulletsStateResponse *state, Bullets *bullets);

void request_log(RequestStructure *req, char *msg, const char *function, const unsigned line);

void response_log(ResponseStructure *res, char* msg, const char* function, const unsigned line);