//
// Created by moonlightnvkz on 19.12.16.
//

#include <assert.h>
#include "text.h"
#include "../defines.h"
#include "../loggers.h"
#include "sdl_helpers.h"

bool text_create(Text *text)
{
    assert(text != NULL);
    text->font = TTF_OpenFont(TEXT_FONT_PATH, TEXT_FONT_SIZE);
    if (text->font == NULL) {
        LOG_ERROR(SDL_GetError());
        return false;
    }
    text->last_text = NULL;
    return true;
}

void text_destroy(Text *text)
{
    TTF_CloseFont(text->font);
    text_free_last_text(text);
}

bool text_render_text(Text *text, char *message, SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b)
{
    text_free_last_text(text);
    SDL_Color color = {r, g, b};
    SDL_Surface *surface = TTF_RenderText_Solid(text->font, message, color);
    if (surface == NULL) {
        LOG_ERROR(SDL_GetError());
        return false;
    }
    text->last_text = SDL_CreateTextureFromSurface(renderer, surface);
    int w, h;
    SDL_QueryTexture(text->last_text, NULL, NULL, &w, &h);
    render_texture(text->last_text, renderer, (WINDOW_WIDTH - w) / 2, (WINDOW_HEIGHT - h) / 2, w, h);
    return true;
}

void text_free_last_text(Text *text)
{
    SDL_DestroyTexture(text->last_text);
    text->last_text = NULL;
}