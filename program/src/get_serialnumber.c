#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>


int get_serialnumber(char *name)
{
	 DIR                *dirp = NULL;
	 char               chip_sn[32];
	 struct dirent      *direntp = NULL;       
         char               w1_path[64] = "/sys/bus/w1/devices/";
	 int                found = 0;
        
         dirp = opendir(w1_path);
	 if(NULL == dirp)
	 {
	    printf("open folader failure:%s\n",strerror(errno));
	    return -1;
	 }
	  
	 while((direntp=readdir(dirp))!=NULL)
	 {
	    if(strstr(direntp->d_name,"28-"))
	     {
	       strncpy(name,direntp->d_name,sizeof(chip_sn));
	       found = 1;
	     }
	 }

	 if(found == 0)
	 {
	   printf("Can not find out serial Number\n");
	   return -3;
	 }
	 closedir(dirp);

}   
