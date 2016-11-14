//
// Created by moonlightnvkz on 14.11.16.
//
#include <SDL2/SDL_image.h>
#include "sdl_helpers.h"

SDL_Texture *load_texture(const char* file, SDL_Renderer *renderer)
{
    SDL_Texture *t = IMG_LoadTexture(renderer, file);
    if (t == NULL) {
        fprintf(stderr, "SDL_CreateTextureFromSurface error: %s", SDL_GetError());
        return NULL;
    }
    return t;
}

void render_texture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}