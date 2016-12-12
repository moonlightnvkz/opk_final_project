//
// Created by moonlightnvkz on 19.11.16.
//

#pragma once

#include <stdbool.h>
#include "../game_logic/geometry.h"
#include "../default_values.h"

typedef struct tPlayer Player;

#pragma pack(push, 1)
typedef struct tStartSignal {
    char symbol;
    unsigned number_of_player;
} StartSignal;

typedef struct tPlayerStateRequest {
    int angle;
    Vector2f position;
    Vector2i velocity;
    bool shot_done;
} PlayerStateRequest;

typedef struct tRequestStructure {
    unsigned req_number;
    PlayerStateRequest player_state;
} RequestStructure;


typedef struct tPlayerStateResponse {
    int angle;
    Vector2f position;
    Vector2i velocity;
} PlayerStateResponse;

typedef struct tBulletStateResponse {
    int angle;
    int ttl;
    Vector2f position;
} BulletStateResponse;

typedef struct tBulletsStateResponse {
    BulletStateResponse bullets[BULLET_MAX_AMOUNT];
    unsigned number;
} BulletsStateResponse;

typedef struct tResponseStructure {
    unsigned res_number;
    PlayerStateResponse players[PLAYER_COUNT];
    BulletsStateResponse bullets;
} ResponseStructure;
#pragma pack(pop)


RequestStructure *request_create(Player *player, unsigned number);

void request_destroy(RequestStructure *req);

void request_log(RequestStructure *req, char *msg, const char *function, const unsigned line);

void response_log(ResponseStructure *res, char* msg, const char* function, const unsigned line);