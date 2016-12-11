//
// Created by moonlightnvkz on 15.11.16.
//

#pragma once

#include <stdbool.h>

bool logger_init();

void logger_destroy();

void log_error(const char *error_message, const char *function, const unsigned line);

void log_action(const char *error_message, const char *function, const unsigned line);