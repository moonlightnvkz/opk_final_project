//
// Created by moonlightnvkz on 15.11.16.
//

#include <SDL_image.h>
#include "initialization.h"
#include "cleaner.h"
#include "default_values.h"
#include "loggers.h"

int initialize(SDL_Window **window, SDL_Renderer **renderer)
{
    logger_init();

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        log_error(SDL_GetError(), __FUNCTION__, __LINE__);
        exit(1);
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        log_error(SDL_GetError(), __FUNCTION__, __LINE__);
        cleanup(NULL, NULL, true, false);
        exit(1);
    }
    *window = SDL_CreateWindow("The Final Project", 100, 100,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        log_error(SDL_GetError(), __FUNCTION__, __LINE__);
        cleanup(NULL, NULL, true, true);
        exit(1);
    }

    *renderer = SDL_CreateRenderer(*window, -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL)
    {
        log_error(SDL_GetError(), __FUNCTION__, __LINE__);
        cleanup(*window, NULL, true, true);
        exit(1);
    }


}