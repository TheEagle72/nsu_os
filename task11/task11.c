#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define ERR_WRONG_USAGE 1
#define ERR_FORK_FAIL 2
#define ERR_MEM_FAIL 3

extern char** environ;

int execvpe_(const char* filename, char** argv, char** envp)
{

	char** env_back = malloc(10);
	putenv(*envp);
	int i = 0;
	while(envp[i])
	{
		env_back[i] = environ[i];
		putenv(envp[i]);
		++i;
	}
	free(envp);
	execvp(filename, argv);
	perror("exec");
	
}

int main(int argc, char** argv)
{
	
	
	if (argc<2)
	{
		printf("usage: %s cmd\n",argv[0]);
		return ERR_WRONG_USAGE;
	}
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("fork failed");
	}

	if (pid > 0)
	{
		wait(NULL);
		execvp(argv[1], argv);
	}
	else
	{
		char** new_environ = malloc(2);
		if (new_environ == NULL)
		{
			return ERR_MEM_FAIL;
		}

		new_environ[0] = "TZ=PST";
		new_environ[1] = NULL;
		execvpe_(argv[1], argv, new_environ);
	}
}
