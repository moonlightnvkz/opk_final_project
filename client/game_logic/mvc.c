//
// Created by moonlightnvkz on 19.11.16.
//

#include <SDL_image.h>
#include "mvc.h"
#include "player.h"
#include "bullet.h"
#include "../server_logic/response_request.h"
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

    mvc->bullets = bullet_create(mvc->renderer);
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
        bullet_destroy(mvc->bullets);
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
    bullet_render_all(mvc->bullets, mvc->renderer);
    SDL_RenderPresent(mvc->renderer);
}

int mvc_process_key(MVC *mvc, const Uint8 *keystates)
{
    if (keystates[SDL_SCANCODE_ESCAPE]) {
        return 1;
    }

    player_keystates_process(mvc->this_player, keystates);

    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        player_do_shot(mvc->this_player, mvc->bullets);
    }
    return 0;
}

void mvc_process_moving(MVC *mvc, unsigned delta_ticks)
{
    player_move(mvc->this_player, delta_ticks);
    player_move(mvc->diff_player, delta_ticks);
    bullet_move_all(mvc->bullets, delta_ticks);
}

void mvc_apply_response(MVC *mvc, Deque *requests_list, ResponseStructure *last_response)
{
    while (((RequestStructure *) deque_peek_first(requests_list)) != NULL &&
           ((RequestStructure *) deque_peek_first(requests_list))->req_number < last_response->res_number) {
        request_destroy((RequestStructure *) deque_remove_first(requests_list));
    }
    if (((RequestStructure *) deque_peek_first(requests_list)) == NULL ||
        ((RequestStructure *) deque_peek_first(requests_list))->req_number != last_response->res_number) {
        return;
    }
    RequestStructure *request = (RequestStructure *) deque_remove_first(requests_list);
    float dx = request->player_state.position.x - last_response->this_player_state.position.x;
    float dy = request->player_state.position.y - last_response->this_player_state.position.y;
    mvc->this_player->geometry.x -= dx;
    mvc->this_player->geometry.y -= dy;
    mvc->diff_player->geometry.x = last_response->diff_player_state.position.x;
    mvc->diff_player->geometry.y = last_response->diff_player_state.position.y;
    mvc->diff_player->velocity.x = last_response->diff_player_state.velocity.x;
    mvc->diff_player->velocity.y = last_response->diff_player_state.velocity.y;
    mvc->diff_player->angle = last_response->diff_player_state.angle;
    char msg[50];
    sprintf(msg, "Req/res positions differs on (%.3f, %.3f)", dx, dy);
    log_action(msg, __FUNCTION__, __LINE__);
    request_destroy(request);
}