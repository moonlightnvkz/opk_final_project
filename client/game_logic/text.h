//
// Created by moonlightnvkz on 19.12.16.
//

#pragma once

#include <SDL_ttf.h>
#include <stdbool.h>

typedef struct tText {
    TTF_Font *font;
    SDL_Texture *last_text;
} Text;

bool text_create(Text *text);

void text_destroy(Text *text);

bool text_render_text(Text *text, char *message, SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b);

void text_free_last_text(Text *text);