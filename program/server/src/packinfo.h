/********************************************************************************
 *      Copyright:  (C) 2023 CKT
 *                  All rights reserved.
 *
 *       Filename:  packinfo.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(05/05/23)
 *         Author:  Cenketao <1697781120@qq.com>
 *      ChangeLog:  1, Release initial version on "05/05/23 17:41:19"
 *                 
 ********************************************************************************/
#ifndef __PACKINFO_H__
#define __PACKINFO_H__

#define NAME_LEN 20
#define TIME_LEN 20
typedef  struct    message_s {
            char    name[NAME_LEN];
            float   temp;
            char    time1[TIME_LEN];
       }message_s;

#endif
