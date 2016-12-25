//
// Created by moonlightnvkz on 24.12.16.
//

#include "globals.h"
#include "game_logic/tile_map.h"
#include "loggers.h"

bool globals_init()
{
    FILE *conf = fopen(GAME_CONF_FILE, "r");
    if (!conf) {
        LOG_ERROR("Failed to open game conf file");
        return false;
    }
    bool error = false;
    if (fscanf(conf, "%d", &GlobalVariables.players_alive_to_end_the_game) != 1) {
        LOG_ERROR("Wrong players_to_end_the_game format");
        error = true;
    }
    fclose(conf);
    return !error;
}