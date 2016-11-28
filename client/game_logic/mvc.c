//
// Created by moonlightnvkz on 19.11.16.
//

#include <SDL_image.h>
#include "mvc.h"
#include "player.h"
#include "bullets.h"
#include "../server_logic/request_response.h"
#include "../loggers.h"
#include "../default_values.h"

MVC *mvc_init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        log_error(SDL_GetError(), __FUNCTION__, __LINE__);
        return NULL;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        log_error(SDL_GetError(), __FUNCTION__, __LINE__);
        return NULL;
    }

    MVC *mvc = malloc(sizeof(MVC));
    if (mvc == NULL) {
        log_error("Failed to allocate memory for gameView", __FUNCTION__, __LINE__);
        mvc_destroy(mvc);
        return NULL;
    }
    mvc->window = SDL_CreateWindow("The Final Project", 100, 100,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (mvc->window == NULL) {
        log_error(SDL_GetError(), __FUNCTION__, __LINE__);
        mvc_destroy(mvc);
        return NULL;
    }

    mvc->renderer = SDL_CreateRenderer(mvc->window, -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (mvc->renderer == NULL) {
        log_error(SDL_GetError(), __FUNCTION__, __LINE__);
        mvc_destroy(mvc);
        return NULL;
    }

    mvc->this_player = player_create(mvc->renderer);
    if (mvc->this_player == NULL) {
        log_error("Failed to allocate memory for this_player", __FUNCTION__, __LINE__);
        mvc_destroy(mvc);
        return NULL;
    }
    mvc->diff_player = player_create(mvc->renderer);
    if (mvc->diff_player == NULL) {
        log_error("Failed to allocate memory for diff_player", __FUNCTION__, __LINE__);
        mvc_destroy(mvc);
        return NULL;
    }

    mvc->bullets = bullets_create(mvc->renderer);
    if (mvc->bullets == NULL) {
        log_error("Failed to allocate memory for bullets", __FUNCTION__, __LINE__);
        mvc_destroy(mvc);
        return NULL;
    }
    return mvc;
}

void mvc_destroy(MVC *mvc)
{
    if (mvc->window != NULL) {
        SDL_DestroyWindow(mvc->window);
    }
    if (mvc->renderer != NULL) {
        SDL_DestroyRenderer(mvc->renderer);
    }
    if (mvc->this_player != NULL) {
        player_destroy(mvc->this_player);
    }
    if (mvc->diff_player != NULL) {
        player_destroy(mvc->diff_player);
    }
    if (mvc->bullets != NULL) {
        bullets_destroy(mvc->bullets);
    }
    SDL_Quit();
    IMG_Quit();
    free(mvc);
}

void mvc_render(MVC *mvc)
{
    SDL_RenderClear(mvc->renderer);
    player_render(mvc->this_player, mvc->renderer);
    player_render(mvc->diff_player, mvc->renderer);
    bullets_render_all(mvc->bullets, mvc->renderer);
    SDL_RenderPresent(mvc->renderer);
}

int mvc_process_key(MVC *mvc, const Uint8 *keystates)
{
    if (keystates[SDL_SCANCODE_ESCAPE]) {
        return MVC_EXIT_KEY_PRESSED;
    }

    player_keystates_process(mvc->this_player, keystates);

    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        player_do_shot(mvc->this_player, mvc->bullets);
    }
    return MVC_NO_ERRORS;
}

void mvc_process_moving(MVC *mvc, unsigned delta_ticks)
{
    player_move(mvc->this_player, delta_ticks);
    player_move(mvc->diff_player, delta_ticks);
    bullets_move_all(mvc->bullets, delta_ticks);
}

void mvc_apply_response(MVC *mvc, Deque *requests_list, ResponseStructure *last_response)
{
    player_apply_response_this(mvc->this_player, requests_list, last_response);
    player_apply_response_diff(mvc->diff_player, last_response);

    bullets_apply_response(mvc->bullets, &last_response->bullets);
}