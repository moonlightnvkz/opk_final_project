//
// Created by moonlightnvkz on 15.11.16.
//

#include <stdio.h>
#include "loggers.h"
#include <stdarg.h>

static FILE* log_file;

bool logger_init()
{
    log_file = fopen("log.log", "w");
    if (!log_file) {
        return true;
    } else {
        return false;
    }
}

void logger_destroy()
{
    fclose(log_file);
}

void log_message(const char *error_message, ...)
{
    if (log_file) {
        va_list valist;
        va_start(valist, error_message);
        va_end(valist);
        vfprintf(log_file, error_message, valist);
    }
}