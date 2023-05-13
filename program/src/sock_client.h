/********************************************************************************
 *      Copyright:  (C) 2023 CKT
 *                  All rights reserved.
 *
 *       Filename:  sock_client.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(09/05/23)
 *         Author:  Cenketao <1697781120@qq.com>
 *      ChangeLog:  1, Release initial version on "09/05/23 23:57:28"
 *                 
 ********************************************************************************/



#ifndef  _SOCK_CLIENT_H_
#define  _SOCK_CLIENT_H_


typedef struct socket_s
{
     int      fd;
	 char     *hostname;//server hostname or ip address
	 int      port;//server listen port

}socket_t;

int sock_init(socket_t *sock_info);

int get_sock_state(socket_t *sock_info);

int sock_connect(socket_t *sock_info);


#endif
