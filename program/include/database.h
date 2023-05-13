/********************************************************************************
 *      Copyright:  (C) 2023 CKT
 *                  All rights reserved.
 *
 *       Filename:  database.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(05/05/23)
 *         Author:  Cenketao <1697781120@qq.com>
 *      ChangeLog:  1, Release initial version on "05/05/23 16:09:20"
 *                 
 ********************************************************************************/
#ifndef __DATABASE_H__
#define __DATABASE_H__
#include<sqlite3.h>
#include <string.h>
#include"packinfo.h"


 int callback(void *NotUsed, int argc, char **argv, char **azColName);

int sqlite_init(char *db_name);

int sqlite_insert(char *dbname,message_s *pack_info);

int get_table(char *dbname,message_s *pack_info);

int sqlite_delect(char *dbname);

int sqlite_close(void);


#endif
