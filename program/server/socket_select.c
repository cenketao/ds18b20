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
#include"database.h"
#include"packinfo.h"
#include"logger.h"
#include"sock_server.h"

#define  ARRAY_SIZE(x)    (sizeof(x)/sizeof(x[0]))

static inline void msleep(unsigned long ms);
static inline void print_usage(char *progname);

int main(int argc,char **argv)
{
	int              listenfd,connefd;
	int              serv_port = 0;
	int              daemon_run = 0;
	char             *progname = NULL;
	int              opt;
	fd_set           rdset;
	int              rv;    
	int              found;
	int              maxfd = 0;
	char             buf[1024];
	int              i,j;
	message_s        pack_info1;
	int              fds_arry[1024];
	char             *db_name="test1";
	char             *q = "/";
	struct option    long_options[] = 
	{
		{"daemon",no_argument,NULL,'b'},
		{"port",required_argument,NULL,'p'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}

	};
	progname = basename(argv[0]);
	/* parser the command line parmeters*/
	while((opt = getopt_long(argc,argv,"bp:h",long_options,NULL)) != -1)
	{
		switch(opt)
		{
			case 'b':
				daemon_run = 1;
				break;
			case 'p':
				serv_port = atoi(optarg);
				break;
			case 'h':
				print_usage(progname);
				return EXIT_SUCCESS;
			default:
				break;
		}


	}

	if(!serv_port)
	{
		print_usage(progname);
		return -1;
	}

	if( logger_init("stdout", LOG_LEVEL_DEBUG) < 0)
	{
		fprintf(stderr, "initial logger system failure\n");
		return -2;
	}

	if((listenfd = socket_server_init(NULL,serv_port))<0)
	{
		log_error("ERROR:%s server listen on port %d  failure\n",argv[0],serv_port);
		return -3;
	}
	log_info("%s server start to listen on port %d\n",argv[0],serv_port);
	/* set program running on background*/
   if(sqlite_init(db_name)<0)
   {
        log_warn("database initial failure!");
   }
   else
   {
        log_info("database initialization ok!");
   }
	if(daemon_run)
	{
		daemon(0,0);
		log_info("running daemon successfully!");
	}

	for(i=0;i<ARRAY_SIZE(fds_arry);i++)
	{
		fds_arry[i] = -1;
	}

	fds_arry[0] = listenfd;
	for( ; ; )
	{
		FD_ZERO(&rdset);
		for(i=0;i<ARRAY_SIZE(fds_arry);i++)
		{
			if(fds_arry[i]<0)
				continue;
			maxfd = fds_arry[i]>maxfd ? fds_arry[i] : maxfd;
			FD_SET(fds_arry[i],&rdset);
		}
		/*program will blocked here*/
		rv = select(maxfd+1,&rdset,NULL,NULL,NULL);
		if(rv<0)
		{
			log_error("select failure :%s\n",strerror(errno));
			break;
		}
		else if(0==rv)
		{
			log_warn(" select get timeout\n");
			continue;
		}
		/*listen socket get event means new client start connect now*/
		if(FD_ISSET(listenfd,&rdset))
		{
			if((connefd=accept(listenfd,(struct sockaddr *)NULL,NULL))<0)
			{
				log_info("accpt new client failure :%s\n",strerror(errno));
				continue;
			}

			found = 0;
			for(i=0;i<ARRAY_SIZE(fds_arry);i++)
			{
				if(fds_arry[i]<0)
				{
					log_info("accept new client[%d] and add it into array\n",connefd);

					fds_arry[i] = connefd;
					found = 1;
					break;
				}
			}
			if(!found)
			{
				log_warn("accept new client[%d] but full ,so refuse it\n",connefd);
				close(connefd);
			}
		}
		else/*data arrive from already connected client*/
		{
			for(i=0;i<ARRAY_SIZE(fds_arry);i++)
			{
				if(fds_arry[i]<0||!FD_ISSET(fds_arry[i],&rdset))
					continue;
				if((rv = read(fds_arry[i],buf,sizeof(buf)))<=0)
				{
					log_warn("socket[%d] read failure or get disconncet.\n",fds_arry[i]);
					fds_arry[i] = -1;
				}
				else
				{
					log_info("From socket[%d]  get some temperature information and save to database\n ",fds_arry[i]);

					snprintf(pack_info1.name,sizeof(pack_info1.name),"%s",strtok(buf,q));
					pack_info1.temp = atoi(strtok(NULL,q));
					snprintf(pack_info1.time1,sizeof(pack_info1.time1),"%s",strtok(NULL,q));

					log_info("%s/%f/%s\n",pack_info1.name,pack_info1.temp,pack_info1.time1);
					sqlite_insert(pack_info1);
					log_info("save to database ok!\n");

				}
			}
		}

	}
	sqlite_close();
CleanUp:
	close(listenfd);
	return 0;
}


static inline void msleep(unsigned long ms)
{
	struct timeval       tv;
	tv.tv_sec = ms/1000;
	tv.tv_usec = (ms%1000)*1000;
	select(0,NULL,NULL,NULL,&tv);
}


static inline void print_usage(char *progname)
{
	printf("Usage : %s[POTION]...\n",progname);
	printf("%s is a socket server program,which used to verify client and echo back string from it\n",progname);
	printf("\nMandatory arguments to long options are mandatory for short options too:\n");
	printf("-b[daemon] set program running on background\n");
	printf("-p[port  ] Socket server port address\n");
	printf("-h[help  ] Display this help information\n ");
	printf("\nExample:%s -b -p 8900\n",progname);
	return ;
}


