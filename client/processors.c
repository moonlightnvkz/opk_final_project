//
// Created by moonlightnvkz on 15.11.16.
//

#include "processors.h"

int process_key(Player *player, const Uint8 *keystates)
{
    if (keystates[SDL_SCANCODE_ESCAPE]) {
        return 1;
    }
    player_keystates_process(player, keystates);
    return 0;
}

void process_moving(Player *player, unsigned delta_ticks)
{
    player_move(player, delta_ticks);
}