/*********************************************************************************
 *      Copyright:  (C) 2023 CKT
 *                  All rights reserved.
 *
 *       Filename:  logger.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(04/05/23)
 *         Author:  Cenketao <1697781120@qq.com>
 *      ChangeLog:  1, Release initial version on "04/05/23 16:40:53"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include "logger.h"

/*
 * Program name variable is provided by the libc
 */
extern const char* __progname;
#define PROGRAM_NAME __progname

/*
 * Logger internal sctructure
 */
typedef struct logger_s {
    FILE   *fp;
    int     loglevel;
    int     use_stdout;
} logger_t;

static struct logger_s g_logger;

static const char* LOG_LEVELS[] = { 
    LOG_STRING_ERROR,
    LOG_STRING_WARN,
    LOG_STRING_INFO,
    LOG_STRING_DEBUG 
};

/*
 * initial logger system
 */ 
int logger_init(char *filename, int loglevel)
{
    logger_term();

    g_logger.loglevel = loglevel>LOG_LEVEL_MAX ? LOG_LEVEL_MAX : loglevel;

    /* $filename is NULL or match "stdout"  will use standard output */
    if( !filename || !strcasecmp(filename, "stdout"))
    {
        g_logger.use_stdout = 1;
        g_logger.fp = stderr;
    }
    else
    {
        g_logger.use_stdout = 0;
        g_logger.fp = fopen(filename, "a");
        if( !g_logger.fp ) 
        {
            fprintf(stderr, "Failed to open file '%s': %s", filename, strerror(errno));
            return -1;
        }
    }

    return 0;
}

/*
 * terminate logger system
 */
void logger_term(void)
{
    if( !g_logger.fp )
    {
        return ;
    }

    if( !g_logger.use_stdout ) 
    {
        fclose(g_logger.fp);
    }

    g_logger.use_stdout = 0;
    g_logger.fp = NULL;

    return ;
}


/*
 * Logging functions
 */
void log_generic(const int level, const char* format, va_list args)
{
    char message[256];
    struct tm* current_tm;
    time_t time_now;

    vsprintf(message, format, args);

    time(&time_now);
    current_tm = localtime(&time_now);

    int res = fprintf(g_logger.fp,
            "%s : %02i:%02i:%02i [%s] %s\n"
                , PROGRAM_NAME
                , current_tm->tm_hour
                , current_tm->tm_min
                , current_tm->tm_sec
                , LOG_LEVELS[level]
                , message );

    fflush(g_logger.fp);
}

void log_error(char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_generic(LOG_LEVEL_ERROR, format, args);
    va_end(args);
}

void log_warn(char *format, ...)
{
    if (g_logger.loglevel < LOG_LEVEL_WARN) {
        return;
    }

    va_list args;
    va_start(args, format);
    log_generic(LOG_LEVEL_WARN, format, args);
    va_end(args);
}

void log_info(char *format, ...)
{
    if (g_logger.loglevel < LOG_LEVEL_INFO) {
        return;
    }

    va_list args;
    va_start(args, format);
    log_generic(LOG_LEVEL_INFO, format, args);
    va_end(args);
}

void log_debug(char *format, ...)
{
    if (g_logger.loglevel < LOG_LEVEL_DEBUG) {
        return;
    }

    va_list args;
    va_start(args, format);
    log_generic(LOG_LEVEL_DEBUG, format, args);
    va_end(args);
}

