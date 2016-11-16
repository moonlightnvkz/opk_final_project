//
// Created by moonlightnvkz on 15.11.16.
//

#include <SDL_mouse.h>
#include "processors.h"

int process_key(Player *player, Bullets *bullets, const Uint8 *keystates)
{
    if (keystates[SDL_SCANCODE_ESCAPE]) {
        return 1;
    }

    player_keystates_process(player, keystates);

    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        player_do_shot(player, bullets);
    }
    return 0;
}

void process_moving(Player *player, Bullets *bullets, unsigned delta_ticks)
{
    player_move(player, delta_ticks);
    bullet_move_all(bullets, delta_ticks);
}