#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define ERR_WRONG_USAGE 1
#define ERR_FORK_FAIL 2
#define ERR_EXEC_FAIL 3
#define ERR_WAIT_FAIL 4

int parent(int pid)
{
	int status;
	if (waitpid(pid, &status, 0) == -1) { return ERR_WAIT_FAIL; }
	if (WIFEXITED(status))
	{
		printf("return code: %d\n", WEXITSTATUS(status));
	}
	return 0;
}
	
int child(int argc, char** argv)
{
	execvp(argv[1], (argv+1));
	printf("exec failed\n");
	exit(ERR_EXEC_FAIL);
}

int main(int argc, char** argv)
{
	if (argc<2)
	{
		printf("usage: %s cmd\n",argv[0]);
		return ERR_WRONG_USAGE;
	}

	pid_t pid = fork();
	
	if (pid <0)
	{
		perror("fork failed!\n");
		return ERR_FORK_FAIL;
	}

	if (pid > 0) 
	{
		parent(pid);
	}
	else
	{
		child(argc, argv);
	}

	return 0;
}
