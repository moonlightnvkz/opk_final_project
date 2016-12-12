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

void render_texture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y, int width, int height)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (width < 0 || height < 0)
    {
        SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    } else {
        dst.w = width;
        dst.h = height;
    }
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void render_texture_ex(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y, int width, int height, double angle)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (width < 0 || height < 0)
    {
        SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    } else {
        dst.w = width;
        dst.h = height;
    }
    SDL_RenderCopyEx(renderer, texture, NULL, &dst, (double) angle, NULL, SDL_FLIP_NONE);
}