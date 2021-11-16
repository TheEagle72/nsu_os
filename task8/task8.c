#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* concat(const char* str1, const char* str2)
{
	char* str = malloc(sizeof(char)*(strlen(str1)+strlen(str2)+1));
	strcpy(str, str1);
	strcat(str, str2);
	return str;
}

int main(int argc, char** argv)
{
	if (argc!=2)
	{
		printf("wrong args\n");
		return 1;
	}

	int fd = open(argv[1], O_RDWR);
	
	if (fd == -1)
	{
		return 2;	
	}
	
	struct flock my_flock;
	my_flock.l_type = F_WRLCK;
	my_flock.l_whence = SEEK_SET;
	my_flock.l_start = 0;
	my_flock.l_len = 0; //all file
	my_flock.l_pid = getpid();
	if (fcntl(fd, F_SETLK, &my_flock)!=-1)
	{
		char* cmd = concat("vim ",argv[1]);
		system(cmd);
		free(cmd);
	}
	else
	{
		char* msg = concat("failed to open ", argv[1]);
		perror(msg);
		free(msg);
	}
	return 0;
	if (!close(fd)) {perror("close failed!");}
}
