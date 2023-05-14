/*********************************************************************************
 *      Copyright:  (C) 2023 CKT
 *                  All rights reserved.
 *
 *       Filename:  ser_sock.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(14/05/23)
 *         Author:  Cenketao <1697781120@qq.com>
 *      ChangeLog:  1, Release initial version on "14/05/23 15:24:58"
 *                 
 ********************************************************************************/
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include <getopt.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

int socket_server_init(char *listen_ip,int listen_port)
{
        struct sockaddr_in        servaddr;
        int                       rv = 0;
        int                       on = 1;
        int                       listenfd;
        if((listenfd = socket(AF_INET,SOCK_STREAM,0))<0)
        {
                printf("Use socket() to create a TCP socket failure :%s\n",strerror(errno));
                return -1;
        }
        /*Set socket port reuseable,fix 'Address already in use' bug when socket server restart*/
        setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

        memset(&servaddr,0,sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(listen_port);

        if(!listen_ip)//listen all the local IP address
        {
                servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        }
        else//liten the specified IP address
        {
                if(inet_pton(AF_INET,listen_ip,&servaddr.sin_addr)<=0)
                {
                        printf("inet_pton() set listen IP address failure.\n");
                        rv = -2;
                        goto CleanUp;
                }
        }
        if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
        {
                printf("Use bind() to bind the TCP socket failure :%s\n",strerror(errno));
                rv = -3;
                goto CleanUp;
        }
        if(listen(listenfd,13)<0)
        {
                printf("Use bind() to bind the TCP socket failure:%s\n",strerror(errno));
                rv = -4;
                goto CleanUp;
        }
CleanUp:
        if(rv<0)
                close(listenfd);
        else
                rv = listenfd;
        return rv;

}

