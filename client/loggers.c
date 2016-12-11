//
// Created by moonlightnvkz on 15.11.16.
//

#include <stdio.h>
#include "loggers.h"
#include "time.h"

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

// Size of the buffer must be equal to 26
static void format_time(char *buffer)
{
    time_t timer;
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
}

void log_error(const char *error_message, const char *function, const unsigned line)
{
    char buffer[26];
    format_time(buffer);
    if (log_file) {
        fprintf(log_file, "%s:Error:%s:%s:%d\n", buffer,error_message, function, line);
    }
}

void log_action(const char *error_message, const char *function, const unsigned line)
{
    char buffer[26];
    format_time(buffer);
    if (log_file) {
        fprintf(log_file, "%s:%s:%s:%d\n", buffer,error_message, function, line);
    }
}