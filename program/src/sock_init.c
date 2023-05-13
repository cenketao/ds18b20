/*********************************************************************************
 *      Copyright:  (C) 2023 Cenketao<1697781120@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  clienttest2.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2023年04月19日)
 *         Author:  Cenketao <1697781120@qq.com>
 *      ChangeLog:  1, Release initial version on "2023年04月19日 14时32分32秒"
 *                 
 ********************************************************************************/
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<getopt.h>
#include<netdb.h>
#include"logger.h"
#include <libgen.h>
#include <linux/tcp.h>
#include"sock_client.h"

int sock_init(socket_t *sock_info)
{
	char                  str[32];
	char                  **pptr;
	struct hostent        *host = NULL;
	struct sockaddr_in    servaddr;
	struct in_addr        inaddr;
	int                   rv = 0;
	//解析IP地址

	if(!inet_aton(sock_info->hostname,&inaddr))//输入域名
	{
		if(NULL==(host = gethostbyname(sock_info->hostname)))
		{
			printf("gethostname error :%s\n",strerror(errno));
			return -1;
		}
		switch(host->h_addrtype)
		{
			case  AF_INET:
			case  AF_INET6:
				pptr = host->h_addr_list;
				for(; *pptr!=NULL;pptr++)

					printf("server IP address : %s\n",inet_ntop(host->h_addrtype,*pptr,str,sizeof(str)));
				printf("%s\n",str);

				strncpy(sock_info->hostname,str,64);
				break;
			default:
				printf("unknown address type\n");
				break;
		}
	 }

	sock_info->fd =socket(AF_INET,SOCK_STREAM,0);

	if(sock_info->fd<0)
	{
		log_error("creat socket failure:%s!",strerror(errno));
		close(sock_info->fd);
	}

	log_info("creat socket[%d] successfully\n",sock_info->fd);

	}


	int sock_connect(socket_t *sock_info)
	{
		struct sockaddr_in    servaddr;
        int                   rv = -1;

		memset(&servaddr,0,sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(sock_info->port);

		inet_aton(sock_info->hostname,&servaddr.sin_addr);//将字符串格式的ip地址转化成网络字节序
		rv = connect(sock_info->fd,(struct sockaddr *)&servaddr,sizeof(servaddr));
		if(rv<0)
		{
			log_error("connect to server[%s:%d]failure :%s!",sock_info->hostname,sock_info->port,strerror(errno));
			close(sock_info->fd);
			return -1;
		}
		printf("connect to server[%s:%d] successfully\n",sock_info->hostname,sock_info->port);


	}

	int get_sock_state(socket_t *sock_info)
	{
		struct tcp_info  info;
		int              len = sizeof(info);
		int              rv = -1;
		if(sock_info->fd<=0)
		{
			return 0;
		}
		memset(&info,0,sizeof(info));

		 getsockopt(sock_info->fd,IPPROTO_TCP,TCP_INFO,&info,(socklen_t*)&len);
		if((info.tcpi_state ==1))
		{
			log_info("socket[%d] connected!",sock_info->fd);
			return 1;
		}
		else
		{
			log_warn("sockfd[%d] disconnected!",sock_info->fd);
	    	return 0;
		}
	}
