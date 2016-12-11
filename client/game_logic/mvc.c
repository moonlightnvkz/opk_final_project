//
// Created by moonlightnvkz on 19.11.16.
//

#include <SDL_image.h>
#include "mvc.h"
#include "../server_logic/request_response.h"
#include "../loggers.h"

bool mvc_init(MVC *mvc)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        log_error(SDL_GetError(), __FUNCTION__, __LINE__);
        return false;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        log_error(SDL_GetError(), __FUNCTION__, __LINE__);
        return false;
    }

    mvc->window = SDL_CreateWindow("The Final Project", 100, 100,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (mvc->window == NULL) {
        log_error(SDL_GetError(), __FUNCTION__, __LINE__);
        mvc_destroy(mvc);
        return false;
    }

    mvc->renderer = SDL_CreateRenderer(mvc->window, -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (mvc->renderer == NULL) {
        log_error(SDL_GetError(), __FUNCTION__, __LINE__);
        mvc_destroy(mvc);
        return false;
    }

    if (!camera_create(&mvc->camera, mvc->renderer)) {
        log_error("Failed  to create camera", __FUNCTION__, __LINE__);
        mvc_destroy(mvc);
        return false;
    }

    if (!tilemap_create(&mvc->map, mvc->renderer)) {
        log_error("Failed to create map", __FUNCTION__, __LINE__);
        mvc_destroy(mvc);
        return false;
    }
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        if (!player_create(mvc->players + i, mvc->renderer)) {
            log_error("Failed to create this_player", __FUNCTION__, __LINE__);
            mvc_destroy(mvc);
            return false;
        }
    }

    if (!bullets_create(&mvc->bullets, mvc->renderer)) {
        log_error("Failed to create bullets", __FUNCTION__, __LINE__);
        mvc_destroy(mvc);
        return false;
    }
    return true;
}

void mvc_destroy(MVC *mvc)
{
    if (mvc->window != NULL) {
        SDL_DestroyWindow(mvc->window);
    }
    if (mvc->renderer != NULL) {
        SDL_DestroyRenderer(mvc->renderer);
    }
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        player_destroy(mvc->players + i);
    }
    tilemap_destroy(&mvc->map);
    bullets_destroy(&mvc->bullets);
    camera_destroy(&mvc->camera);
    SDL_Quit();
    IMG_Quit();
}

void mvc_render(MVC *mvc)
{
    SDL_RenderClear(mvc->renderer);
    tilemap_render(&mvc->map, mvc->renderer, &mvc->camera);
    Vector2f this_player_pos = {mvc->players[GlobalVariables.number_of_player].geometry.x,
                                mvc->players[GlobalVariables.number_of_player].geometry.y};

    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        player_render(mvc->players + i, mvc->renderer, &mvc->camera);
        if (i != GlobalVariables.number_of_player) {
            Vector2f another_player_pos = {mvc->players[i].geometry.x,
                                           mvc->players[i].geometry.y};
            camera_render_pointing_arrow(&mvc->camera, mvc->renderer, this_player_pos, another_player_pos);
        }
    }
    bullets_render_all(&mvc->bullets, mvc->renderer, &mvc->camera);
    SDL_RenderPresent(mvc->renderer);
}

int mvc_process_key(MVC *mvc, const Uint8 *keystates)
{
    if (keystates[SDL_SCANCODE_ESCAPE]) {
        return MVC_EXIT_KEY_PRESSED;
    }

    player_angle_process(mvc->players + GlobalVariables.number_of_player, &mvc->camera);

    player_keystates_process(mvc->players + GlobalVariables.number_of_player, keystates);

    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        player_do_shot(mvc->players + GlobalVariables.number_of_player, &mvc->bullets);
    }
    return MVC_NO_ERRORS;
}

void mvc_process_moving(MVC *mvc, unsigned delta_ticks)
{
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        player_move(mvc->players + i, delta_ticks);
    }
    bullets_move_all(&mvc->bullets, delta_ticks);

    camera_move_after_the_player(&mvc->camera, mvc->players + GlobalVariables.number_of_player);
}

void mvc_apply_response(MVC *mvc, Deque *requests_list, ResponseStructure *last_response)
{
    player_apply_response_this(mvc->players + GlobalVariables.number_of_player, requests_list, last_response);
    player_apply_response_others(mvc->players, last_response);

    bullets_apply_response(&mvc->bullets, &last_response->bullets);
}