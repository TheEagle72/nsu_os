#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define ERR_WRONG_USAGE 1
#define ERR_FORK_FAIL 2
#define ERR_EXEC_FAIL 3

int parent(void)
{
	printf("parent started\n");
	wait(NULL);
	printf("parent finished\n");
	return 0;
}

int child(int argc, char** argv)
{
	execlp("/bin/cat", "cat", argv[1], NULL);
	printf("exec failed\n");
	exit(ERR_EXEC_FAIL);
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("usage: %s file_name\n",argv[0]);
		return ERR_WRONG_USAGE;
	}

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
		child(argc, argv);
	}
	return 0;
}
