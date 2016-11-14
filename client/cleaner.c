//
// Created by moonlightnvkz on 14.11.16.
//

#include "cleaner.h"
#include <SDL2/SDL_image.h>

void cleanup(SDL_Window *window, SDL_Renderer *renderer, bool sdl_quit, bool img_quit)
{
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (sdl_quit) {
        SDL_Quit();
    }
    if (img_quit) {
        IMG_Quit();
    }
    SDL_Quit();
}