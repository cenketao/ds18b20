/********************************************************************************
 *      Copyright:  (C) 2023 CKT
 *                  All rights reserved.
 *
 *       Filename:  logger.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(04/05/23)
 *         Author:  Cenketao <1697781120@qq.com>
 *      ChangeLog:  1, Release initial version on "04/05/23 16:58:20"
 *                 
 ********************************************************************************/
#ifndef  _LOGGER_H_
#define  _LOGGER_H_

/*
 * logger level
 */
enum 
{
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_MAX,
};

/*
 * logger prefix string for different logging levels
 */
#define LOG_STRING_ERROR  "ERROR"
#define LOG_STRING_WARN   "WARN "
#define LOG_STRING_INFO   "INFO "
#define LOG_STRING_DEBUG  "DEBUG"


/*
 * logger initial and terminate functions
 */
int logger_init(char *filename, int loglevel);
void logger_term(void);

/*
 * logging methods by levels
 */
void log_error(char* format, ...);
void log_warn(char* format, ...);
void log_info(char* format, ...);
void log_debug(char* format, ...);

#endif   /* ----- #ifndef _LOGGER_H_  ----- */

