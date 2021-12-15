#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ERR_FORK_FAIL 1


int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("usage: %s *text*\n", argv[0]);
		return EXIT_SUCCESS;
	}

	int pipefd[2];
	char buf;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid_t sender = fork();

	if (sender == -1)
	{
		return ERR_FORK_FAIL;
	}

	if (sender == 0)
	{
			//sender code
			close(pipefd[0]);
			write(STDOUT_FILENO, argv[1], strlen(argv[1])); 
			write(STDOUT_FILENO, "\n", 1); 
			write(pipefd[1], argv[1], strlen(argv[1]));
			close(pipefd[1]);
			exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(sender, NULL, 0);

		pid_t reciever = fork();
		
		if (reciever < 0)
		{
			return ERR_FORK_FAIL;
		}

		if (reciever == 0)
		{
			//receiver code
			close(pipefd[1]);
			while (read(pipefd[0], &buf, 1) > 0)
			{
				buf = toupper(buf);
				write(STDOUT_FILENO, &buf, 1);
			}
			write(STDOUT_FILENO, "\n", 1); 
			close(pipefd[0]);
			exit(EXIT_SUCCESS);
		}
		else
		{
			waitpid(reciever, NULL, WNOHANG);
		}
	}
	return 0;
}	
