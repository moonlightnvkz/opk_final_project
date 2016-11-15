//
// Created by moonlightnvkz on 15.11.16.
//

#pragma once

#include <SDL_types.h>
#include "player.h"

int process_key(Player *player, const Uint8 *keystates);

void process_moving(Player *player, unsigned delta_ticks);