#include<stdio.h>
#include<string.h>
#include<time.h>
#include <sys/time.h>

long get_time(char *buf)
{
	  long              rtime;
      time_t            tmp;
      struct tm         *timp;
      struct timeval    tv;

	  gettimeofday(&tv,NULL);

	  rtime = tv.tv_sec;
      time(&tmp);

      timp = localtime(&tmp);

     snprintf(buf,32,"%d-%02d-%02d %02d:%02d:%02d",
			 (1900+timp->tm_year),(1+timp->tm_mon),timp->tm_mday,timp->tm_hour,timp->tm_min,timp->tm_sec);

 return rtime;
}
