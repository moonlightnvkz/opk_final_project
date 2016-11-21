//
// Created by moonlightnvkz on 19.11.16.
//

#pragma once

#include <stdbool.h>
#include "../game_logic/geometry.h"

typedef struct tPlayer Player;

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
    bool shot_done;
} PlayerStateResponse;

typedef struct tResponseStructure {
    unsigned res_number;
    PlayerStateResponse this_player_state;
    PlayerStateResponse diff_player_state;
} ResponseStructure;
#pragma pack(pop)


RequestStructure *request_create(Player *player, unsigned number);

void request_destroy(RequestStructure *req);

void request_log(RequestStructure *req, char *msg, const char *function, const unsigned line);

void response_log(ResponseStructure *res, char* msg, const char* function, const unsigned line);