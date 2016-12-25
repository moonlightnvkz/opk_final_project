//
// Created by moonlightnvkz on 19.11.16.
//

#pragma once

#include <stdbool.h>
#include "../game_logic/geometry.h"
#include "../defines.h"

typedef struct tMVC MVC;

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
    unsigned description;   // player number
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

typedef struct tExplosiveStateResponse {
    Vector2i position_at_map;   // map_descr[][]
    bool is_damaged;
    int timer_damaged;
    bool is_exploding;
    int timer_explosion;
} ExplosiveStateResponse;

typedef struct tExplosivesStateResponse {
    ExplosiveStateResponse explosives[EXPLOSIVE_MAX_AMOUNT];
    unsigned number;
} ExplosivesStateResponse;

typedef struct tResponseStructure {
    bool quit;
    unsigned res_number;
    PlayerStateResponse players[PLAYER_COUNT];
    BulletsStateResponse bullets;
    ExplosivesStateResponse explosives;
} ResponseStructure;
#pragma pack(pop)


RequestStructure *request_create(MVC *mvc, unsigned number);

void request_destroy(RequestStructure *req);

void request_log(RequestStructure *req, char *msg, const char *function, const unsigned line);

void response_log(ResponseStructure *res, char* msg, const char* function, const unsigned line);