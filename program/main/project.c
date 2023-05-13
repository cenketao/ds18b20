/*********************************************************************************
 *      Copyright:  (C) 2023 CKT
 *                  All rights reserved.
 *
 *       Filename:  project.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(21/04/23)
 *         Author:  Cenketao <1697781120@qq.com>
 *      ChangeLog:  1, Release initial version on "21/04/23 21:44:25"
 *                 
 ********************************************************************************/
#include"gettime.h"
#include"gettemp.h"
#include"packinfo.h"
#include"logger.h"
#include"sock_client.h"
#include<stdio.h>
#include<errno.h>
#include<getopt.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>     
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<libgen.h>
#include<linux/tcp.h>
#include"database.h"
#include<time.h>
#include <sys/time.h>

void print_usage(char *progname)
{
	printf("%s usage:\n",progname);
	printf("You input ip address or domain one of two\n");
	printf("--ipaddr(-i):sepcify server ipaddress or hostname\n");
	printf("--port(-p):sepcify server POTR\n");
	printf("--time(-t):interval time(s) to get temperature\n");
	printf("--help(-h):printf help information\n");
	return ;

}
int get_sn(char *name)
{
	int sn = 2;
	snprintf(name,16,"CKT%04d",sn);
	return 0;
}

int g_stop = 0;

int main(int argc,char **argv)
{
	char                 *dbname = "ckt.db";
	int                   socket_connected = 0;
	int                   flag = 0;
	long                  now_time = 0;
	long                  pick_time = 0;
	int                   rv=-1;
	int                   interval_time = -1;
	struct timeval        tv;
	socket_t              sock_init1={-1,"127.0.0.1",8089};
	socket_t              *sock_info = &sock_init1;
	char                  save__buf[128];
	char                  buf1[128];
	char                  buf2[128];
	message_s             pack_info1 ;
	struct option         opts[] = {
		{"ipaddr",required_argument,NULL,'i'},
		{"port",required_argument,NULL,'p'},
		{"domain",required_argument,NULL,'d'},
		{"time",required_argument,NULL,'t'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}
	};
	int                    ch;

	while((ch=getopt_long(argc,argv,"i:p:t:h",opts,NULL))!=-1)
	{
		switch(ch)
		{
			case 'i':
				sock_info->hostname = optarg;
				break;
			case 'p':
				sock_info->port=atoi(optarg);
				break;
			case 't':
				interval_time = atoi(optarg);
				break;
			case 'h':
				print_usage(argv[0]);
				break;
			default:
				printf("unknow return val %d\n",ch);
		}

	}

	if(logger_init("running.log",LOG_LEVEL_DEBUG)<0)
	{
		fprintf(stderr,"initial logger system failure\n");
		return -2;
	}

	if(!interval_time && !sock_info->hostname && !port)
	{
		print_usage(argv[0]);
		return -3;
	}


	if((rv = sock_init(sock_info))<0)
	{
		log_error("socket initial  failure!");
	}

	if((rv = sock_connect(sock_info))<0)
	{
		log_error("socket first connect failure!");
		close(sock_info->fd);
		sock_info->fd = -1;
	}

	if((rv = sqlite_init(dbname))<0)
	{
		log_error("database initial false!");
		return -6;
	}  

	while(!g_stop)

	{
		gettimeofday(&tv,NULL);
		now_time = tv.tv_sec;

		if((now_time-pick_time)>=interval_time)
		{
			flag = 1;//sample data

			if((rv=get_sn(pack_info1.name))<0)
			{
				log_error("get serialnumber failure!");
				return -4;
			}

			if((rv=get_temperature(&pack_info1.temp))<0)
			{
				log_error("get temperature failure!");
				return -5;
			}

			if((pick_time=get_time(pack_info1.time1))<0)
			{
				log_error("get time failure!");
				return -6;
			}

			memset(buf1,0,sizeof(buf1));
			snprintf(buf1,sizeof(buf1),"%s/%f/%s\n",pack_info1.name,pack_info1.temp,pack_info1.time1);
			printf("%s/%f/%s\n",pack_info1.name,pack_info1.temp,pack_info1.time1);

		}

		else
		{
			flag = 0;
			log_info("It  is not time to sample! ");
		}

		socket_connected = get_sock_state(sock_info);//返回0表示断开socke,返回1表示socket连接
		signal(SIGPIPE,SIG_IGN);

		if(socket_connected)
		{  

			if((rv = get_table(save__buf))<0)
			{         
				log_error("get table failure!\n");
			}
			else
			{
				memset(buf2,0,sizeof(buf2));
				snprintf(buf2,sizeof(buf2),"%s",save__buf);

				if( rv = write(sock_info->fd,buf2,strlen(buf2))<0)
				{
					log_error("database write to server failure!");
				}
				else
				{
					log_info("database write to server successfully!");
				}

				if(( rv = sqlite_delect())<0)
				{
					log_error("database delect failure!");
				}
				else
				{
					log_info("DELETE sqlite database successfully!");
				}

			}

			if(flag)
			{
				if(	rv = write(sock_info->fd,buf1,strlen(buf1))<0);
				{
					log_error("socket rite to server failure!");

					sqlite_insert(pack_info1);

					log_error("INSERT to database failure!");
				}
			}
		}

		if(!socket_connected)
		{
			close(sock_info->fd);

			sock_info->fd = -1;

			rv = sock_init(sock_info);

			if((rv = sock_connect(sock_info))<0)
			{
				log_error("socket restart connect failure!");
				close(sock_info->fd);
				sock_info->fd = -1;
				log_warn("Breaking and write to database!");
			}
			else
			{
				log_info("socket restart connect successfully!");
				socket_connected = 1;
			}

			if(flag)
			{	  
				rv = sqlite_insert(pack_info1);

				if(rv<0)
				{
					log_error("INSERT to database failure!");
				}
			}

		}

		sleep(2);
	}
	sqlite_close();
	logger_term();
}


