//
// Created by moonlightnvkz on 15.11.16.
//

#pragma once

#include <stdbool.h>

#define LOG_ERROR(fmt, ...) MY_LOGGER1(Error:fmt, ##__VA_ARGS__)
#define LOG_ACTION(fmt, ...) MY_LOGGER1(Action:fmt, ##__VA_ARGS__)
#define MY_LOGGER1(fmt, ...) MY_LOGGER2(%s:%d:fmt\n, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define MY_LOGGER2(fmt, ...) do {log_message(#fmt, ##__VA_ARGS__);} while(0);

bool logger_init();

void logger_destroy();

void log_message(const char *error_message, ...);