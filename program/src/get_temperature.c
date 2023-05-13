#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>


int  get_temperature(float *temp)
{
        int               fd = -1;
	char              buf[128];
	char              *ptr = NULL;
	DIR               *dirp = NULL;
	struct dirent     *direntp = NULL;
	char              w1_path[64] = "/sys/bus/w1/devices/";
        char              chip_sn[32];
	int               found = 0;

	dirp = opendir(w1_path);
	if(NULL == dirp)
	{
	   printf("open folader device %s failure :%s\n",w1_path,strerror(errno));
	   return -2;
	}//open 文件夹

	if(NULL == (direntp=readdir(dirp)))
        {
	   printf("read folader device %s failure:%s\n",w1_path,strerror(errno));
	   return -3;
	}
	while(NULL != (direntp=readdir(dirp)))
	{
	   if(strstr(direntp->d_name,"28-"))
	   {
	      strncpy(chip_sn,direntp->d_name,sizeof(chip_sn));
	      found = 1; 
	   }
	}//read 文件夹并获取产品序列号
	closedir(dirp);
       if(found==0)
       {
         printf("Can not find ds18b20 chipset\n");
	 return -4;
       }

       strncat(w1_path,chip_sn,sizeof(w1_path)-strlen(w1_path));
       strncat(w1_path,"/w1_slave",sizeof(w1_path)-strlen("w1_path"));

       fd = open(w1_path,O_RDONLY);
       if(fd<0)
       {
         printf("open file failure :%s\n",strerror(errno));
	 return -5;
       }

       memset(buf,0,sizeof(buf));

       if(read(fd,buf,sizeof(buf))<0)
       {
         printf("read date from device failure :%s\n",strerror(errno));
	 return -6;
       }

       ptr = strstr(buf,"t=");
       if(ptr == NULL)
       {
         printf("Can not find t= string\n");
         return -7;	 
       }
       
       ptr +=2;

       *temp = atof(ptr)/1000;
       
//       printf("temperature:%f\n",*temp);

       close(fd);
}
