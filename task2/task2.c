#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
extern char *tzname[];

int main()
{
time_t now;
struct tm *sp;

time( &now );


putenv("PST");
//putenv("TZ=America/Tijuana");
//putenv("TZ=America/Los_Angeles");
//time zones can be found in /usr/share/zoneinfo/posix/ 
printf("%s", ctime( &now ) );

sp = localtime(&now);
printf("%d/%d/%02d %d:%02d %s\n",
	sp->tm_mon + 1, sp->tm_mday,
	sp->tm_year, sp->tm_hour,
	sp->tm_min, tzname[sp->tm_isdst]);
return 0;
}

