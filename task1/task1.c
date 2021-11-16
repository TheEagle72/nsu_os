#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ulimit.h>
#include <limits.h>
 
int main(int argc, char** argv)
{
char valid_args[] = "ispuU:d";
int arg;
char cwd [PATH_MAX];

while ((arg=getopt(argc,argv,valid_args)) != EOF)
switch (arg)
{
case 'i':
   printf("real_id: %d\neffective_id: %d\n",getuid(),geteuid());
   break;
case 's':
   setpgrp(); 
   break;
case 'p':
   printf("process_id: %d\nparent-process_id: %d\nprocess_group_id: %d\n",getpid(), getpid(), getpgid(0));
   break;
case 'u':
   printf("user_limit: %ld",ulimit(UL_GETFSIZE)); 
   break;
case 'U':
   printf("%s\n",optarg);
   ulimit(UL_SETFSIZE,optarg);
   break;
case 'd':
   getcwd(cwd,sizeof(cwd));
   printf("current_working_directory: %s\n",cwd);
   break;
case '?':
   //printf("wrong argument: %c\n",optopt);
   break;
}
return 0;
}
