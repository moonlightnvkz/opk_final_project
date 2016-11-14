//
// Created by moonlightnvkz on 14.11.16.
//

#pragma once

#include <SDL2/SDL.h>

void render_texture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y);

SDL_Texture *load_texture(const char* file, SDL_Renderer *renderer);