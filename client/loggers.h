//
// Created by moonlightnvkz on 15.11.16.
//

#pragma once

int logger_init();

void logger_destroy();

void log_error(const char *error_message, const char *function, const unsigned line);