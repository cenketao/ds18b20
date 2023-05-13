/********************************************************************************
 *      Copyright:  (C) 2023 CKT
 *                  All rights reserved.
 *
 *       Filename:  project.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(21/04/23)
 *         Author:  Cenketao <1697781120@qq.com>
 *      ChangeLog:  1, Release initial version on "21/04/23 21:53:19"
 *                 
 ********************************************************************************/

#ifndef _PROJECT_H_
#define _PROJECT_H_
#include"packinfo.h"

int get_time(char *buf);
int get_temperature(float *temp);
int get_serialnumber(char *name);
int sock_init(char *servip,char *domain,int port);

#endif

