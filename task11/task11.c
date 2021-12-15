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
	char** tmp = environ;
	environ = envp;
	execvp(filename, argv);
	perror("exec");
	environ = tmp;

/*
	int size=0;
	while (environ[size++]);
	char** env_back=malloc(size*sizeof(char*));
	int i = 0;

	while(environ[i])
	{
		env_back[i] = malloc(strlen(environ[i])+1);
		if (!env_back[i])
		{
			perror("malloc");
			return -1;
		}
		strcpy(env_back[i], environ[i]);
		++i;
	}
	
	i=0;
	while(envp[i])
	{
		putenv(envp[i]);
		++i;
	}
	free(envp);
	execvp(filename, argv);
	
	//if failed
	i = 0;
	while(env_back[i])
	{
		putenv(env_back[i++]);
	}
	perror("exec");
*/	
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
	return 0;
}
