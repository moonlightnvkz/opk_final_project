//
// Created by moonlightnvkz on 19.11.16.
//

#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../server_logic/request_response.h"
#include "mvc.h"
#include "../loggers.h"

bool mvc_init(MVC *mvc)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        LOG_ERROR(SDL_GetError());
        return false;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        LOG_ERROR(SDL_GetError());
        return false;
    }

    if (TTF_Init() != 0) {
        LOG_ERROR(SDL_GetError());
        return false;
    }

    mvc->window = SDL_CreateWindow("The Final Project", 100, 100,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (mvc->window == NULL) {
        LOG_ERROR("SDL_GetError()");
        mvc_destroy(mvc);
        return false;
    }

    mvc->renderer = SDL_CreateRenderer(mvc->window, -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (mvc->renderer == NULL) {
        LOG_ERROR("SDL_GetError()");
        mvc_destroy(mvc);
        return false;
    }

    if (!camera_create(&mvc->camera, mvc->renderer)) {
        LOG_ERROR("Failed  to create camera");
        mvc_destroy(mvc);
        return false;
    }

    if (!tilemap_create(&mvc->map, mvc->renderer)) {
        LOG_ERROR("Failed to create map");
        mvc_destroy(mvc);
        return false;
    }
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        if (!player_create(mvc->players + i, mvc->renderer)) {
            LOG_ERROR("Failed to create this_player");
            mvc_destroy(mvc);
            return false;
        }
    }

    if (!bullets_create(&mvc->bullets, mvc->renderer)) {
        LOG_ERROR("Failed to create bullets");
        mvc_destroy(mvc);
        return false;
    }

    if (!text_create(&mvc->text)) {
        LOG_ERROR("Failed to load text");
        mvc_destroy(mvc);
        return false;
    }

    mvc->criticalEvent.type = CE_NONE;
    mvc->criticalEvent.description = 0;
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
    text_destroy(&mvc->text);
    bullets_destroy(&mvc->bullets);
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        player_destroy(mvc->players + i);
    }
    tilemap_destroy(&mvc->map);
    camera_destroy(&mvc->camera);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void mvc_render(MVC *mvc)
{
    SDL_RenderClear(mvc->renderer);
    tilemap_render(&mvc->map, mvc->renderer, &mvc->camera);
    Vector2f pa_from = {mvc->players[GlobalVariables.number_of_player].geometry.x +
                                mvc->players[GlobalVariables.number_of_player].geometry.width  / 2.f,
                        mvc->players[GlobalVariables.number_of_player].geometry.y +
                                mvc->players[GlobalVariables.number_of_player].geometry.height / 2.f};

    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        player_render(mvc->players + i, mvc->renderer, &mvc->camera);
        if (i != GlobalVariables.number_of_player) {
            Vector2f pa_to = {mvc->players[i].geometry.x + mvc->players[i].geometry.width  / 2.f,
                              mvc->players[i].geometry.y + mvc->players[i].geometry.height / 2.f};
            camera_render_pointing_arrow(&mvc->camera, mvc->renderer, pa_from, pa_to);
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
        if (player_do_shot(mvc->players + GlobalVariables.number_of_player, &mvc->bullets)) {
            mvc->criticalEvent.type = CE_SHOT_DONE;
            mvc->criticalEvent.description = GlobalVariables.number_of_player;
        }
    }
    return MVC_NO_ERRORS;
}

void mvc_process_moving(MVC *mvc, unsigned delta_ticks)
{
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        player_move(mvc->players + i, delta_ticks);
    }

    bool alive_flag_states[PLAYER_COUNT];
    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        alive_flag_states[i] = mvc->players[i].is_alive;
    }

    bullets_move_all(&mvc->bullets, delta_ticks, mvc->players);
    explosives_explode_process(&mvc->map.explosives, delta_ticks, mvc->players);

    for (unsigned i = 0; i < PLAYER_COUNT; ++i) {
        if (alive_flag_states[i] != mvc->players[i].is_alive) {
            mvc->criticalEvent.type = CE_DEATH;
            mvc->criticalEvent.description = i;
            break;
        }
    }
    //camera_move_after_the_player(&mvc->camera, mvc->players + GlobalVariables.description);
    camera_move_after_the_mouse(&mvc->camera, &mvc->players[GlobalVariables.number_of_player]);
}

void mvc_apply_response(MVC *mvc, Deque *requests_list, ResponseStructure *last_response)
{
    GlobalVariables.quit = last_response->quit;

    player_apply_response_this(mvc->players + GlobalVariables.number_of_player, requests_list, last_response);
    player_apply_response_others(mvc->players, last_response);

    bullets_apply_response(&mvc->bullets, &last_response->bullets);

    explosives_apply_response(&mvc->map.explosives, &last_response->explosives);
}