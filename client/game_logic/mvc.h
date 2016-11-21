//
// Created by moonlightnvkz on 19.11.16.
//

#pragma once

#include <SDL_render.h>
#include "../my_deque.h"

typedef struct tPlayer Player;
typedef struct tBullets Bullets;
typedef struct tResponseStructure ResponseStructure;

typedef enum eMvcReturnCodes {
    MVC_NO_ERRORS,
    MVC_EXIT_KEY_PRESSED,
    MVC_SHOT_DONE
} MvcReturnCodes;

typedef struct tMVC {
    SDL_Window *window;
    SDL_Renderer *renderer;

    Player *this_player;
    Player *diff_player;
    Bullets *bullets;
} MVC;

MVC *mvc_init();

void mvc_destroy(MVC *mvc);

void mvc_render(MVC *mvc);

int mvc_process_key(MVC *mvc, const Uint8 *keystates);

void mvc_process_moving(MVC *mvc, unsigned delta_ticks);

void mvc_apply_response(MVC *mvc, Deque *requests_list, ResponseStructure *last_response);