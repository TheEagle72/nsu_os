#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/file.h>

#define ERR_EXEC_FAIL 1
#define ERR_FORK_FAIL 2
#define ERR_LOCKED 3

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

	if (fcntl(fd, F_SETLK, &my_flock) == -1)
	//if (flock(fd, LOCK_EX) == -1)
	{
		perror("lock failed");
		return ERR_LOCKED;
	}

	system("vim data");
	//execlp("vim", "vim",  argv[1], NULL);
	
	my_flock.l_type = F_UNLCK;
	if (fcntl(fd, F_SETLK, &my_flock) == -1)
	//if (flock(fd, LOCK_UN) == -1)
	{
		perror("unlock failed");
		return ERR_LOCKED;
	}
	
	if (close(fd)) {perror("close failed!");}
	return 0;
}
