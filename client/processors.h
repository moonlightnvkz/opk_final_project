//
// Created by moonlightnvkz on 15.11.16.
//

#pragma once

#include <SDL_types.h>
#include "player.h"
#include "bullet.h"

int process_key(Player *player, Bullets *bullets, const Uint8 *keystates);

void process_moving(Player *player, Bullets *bullets, unsigned delta_ticks);