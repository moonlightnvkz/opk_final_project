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
typedef struct tStartSignal {
    char symbol;
    unsigned number_of_player;
} StartSignal;

typedef struct tPlayerStateRequest {
    double angle;
    Vector2f position;
    Vector2i velocity;
} PlayerStateRequest;

typedef enum eCriticalEventType {
    CE_NONE,
    CE_SHOT_DONE,
    CE_DEATH
} CriticalEventType;

typedef struct tCriticalEvent {
    CriticalEventType type;
    unsigned number_of_player;  // player which done event
} CriticalEvent;

typedef struct tRequestStructure {
    unsigned req_number;
    PlayerStateRequest player_state;
    CriticalEvent critical_event;
} RequestStructure;

typedef struct tPlayerStateResponse {
    double angle;
    Vector2f position;
    Vector2i velocity;
    bool is_alive;
} PlayerStateResponse;

typedef struct tBulletStateResponse {
    double angle;
    int ttl;
    Vector2f position;
} BulletStateResponse;

typedef struct tBulletsStateResponse {
    BulletStateResponse bullets[BULLET_MAX_AMOUNT];
    unsigned number;
} BulletsStateResponse;

typedef struct tResponseStructure {
    bool quit;
    unsigned res_number;
    PlayerStateResponse players[PLAYER_COUNT];
    BulletsStateResponse bullets;
} ResponseStructure;
#pragma pack(pop)

void response_set_player_states(PlayerStateResponse *state, Player *player);

void response_set_bullets_states(BulletsStateResponse *state, Bullets *bullets);

void request_log(RequestStructure *req, char *msg, const char *function, const unsigned line);

void response_log(ResponseStructure *res, char* msg, const char* function, const unsigned line);