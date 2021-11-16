#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define ERR_FORK_FAIL 1

int parent(void)
{
	wait(NULL);
	printf("parent\n");

	return 0;
}

int child(void)
{
	printf("child\n");
	system("cat data");
	return 0;
}

int main()
{
	pid_t pid = fork();
	
	if (pid <0)
	{
		perror("fork failed!\n");
		return ERR_FORK_FAIL;
	}

	if (pid > 0) //parent
	{
		parent();
	}
	else
	{
		child();
	}
	return 0;
}
