#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define ERR_WRONG_USAGE 1
#define ERR_FORK_FAIL 2
#define ERR_FAILURE 3

int main(int argc, char** argv)
{
	pid_t pid = fork();
	
	if (pid <0)
	{
		perror("fork failed!\n");
		return ERR_FORK_FAIL;
	}

	if (pid > 0) 
	{
		sleep(30);
	}
	else
	{
		exit(0);
	}

	return 0;
}
