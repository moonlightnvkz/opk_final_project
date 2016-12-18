//
// Created by moonlightnvkz on 19.11.16.
//

#pragma once

#include <SDL_render.h>
#include <stdbool.h>
#include "../my_deque.h"
#include "../default_values.h"
#include "camera.h"
#include "tile_map.h"
#include "player.h"
#include "bullets.h"
#include "../server_logic/request_response.h"
#include "explosive.h"

typedef struct tResponseStructure ResponseStructure;

typedef enum eMvcReturnCodes {
    MVC_NO_ERRORS,
    MVC_EXIT_KEY_PRESSED
} MvcReturnCodes;

typedef struct tMVC {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Camera camera;

    TileMap map;
    Player players[PLAYER_COUNT];
    Bullets bullets;
    CriticalEvent criticalEvent;
} MVC;

bool mvc_init(MVC *mvc);

void mvc_destroy(MVC *mvc);

void mvc_render(MVC *mvc);

int mvc_process_key(MVC *mvc, const Uint8 *keystates);

void mvc_process_moving(MVC *mvc, unsigned delta_ticks);

void mvc_apply_response(MVC *mvc, Deque *requests_list, ResponseStructure *last_response);