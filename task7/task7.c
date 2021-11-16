#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <poll.h>
#include <sys/mman.h>

#define ERR_SUCCESS 0
#define ERR_WRONG_ARGS 1
#define ERR_FILE_OPEN_FAILED 2
#define ERR_INVALID_SIZE 3

#define DEFAULT_COUNT 8
#define INCREMENT_COUNT 8

#define TIMEOUT 5000

typedef struct table
{
	size_t size;
	off_t* indices;
} table;


typedef int error_type;

off_t file_get_size(off_t fd);
off_t file_open(const char* path);
char* file_read(const off_t fd, const size_t size);
void file_close(const off_t fd);
off_t get_number(void);


off_t file_get_size(const off_t fd)
{
	if (fd==-1)
	{
		return 0;
	}

	off_t file_size = lseek(fd, 0L, SEEK_END);
	lseek(fd, 0L, SEEK_SET);
	return file_size;
}

off_t file_open(const char* path)
{
	
	//trying to open file
	off_t file_descriptor = open(path, O_RDONLY); // file descriptor
	if (file_descriptor == -1)
	{
		perror("file can't be opened");
		return -1;
	}

	return file_descriptor;
}

char* file_read(const off_t fd, const size_t size)
{
	if (size < 0)
	{
		return NULL;
	}

	char* buffer = malloc(sizeof(char)*size);
	read(fd, buffer, size);
	return buffer;
}

void file_close(const off_t fd)
{
	if (close(fd))
	{
		perror("close failed\n");
	}
}

off_t get_number(void)
{
	off_t line_number;
	while (scanf("%ld", &line_number)!=1)
	{
		while (getchar()!='\n'); //clear buffer
	}
	return line_number;
}

table file_parse(const char* buffer, const size_t size)
{
	size_t capacity = DEFAULT_COUNT;
	size_t count = 0; // count of '\n'
	off_t* indices = malloc(sizeof(off_t)*capacity);

	for (size_t i = 0; i < size; ++i) 
	{
		if (buffer[i] == '\n')
		{
			if (capacity==count)
			{
				capacity += INCREMENT_COUNT;
				off_t* tmp = realloc(indices, sizeof(off_t)*capacity);
				if (!tmp)
				{
					perror("MEMORY ALLOCATION FAILED");
					free(indices);
					table t = {0, NULL};
					return t;
				}
				indices = tmp;
			}
			++count;
			indices[count] = i;
		}
	}
	table t = {count, indices};
	return t;
}

void file_get_string(const char* buffer, const off_t buffer_size, const table t)
{
	char* str = NULL;
	off_t index = 0;
	printf("count of lines: %ld\n", t.size);
	printf("line number: ");
	fflush(stdout);

	struct pollfd pdfs;
	pdfs.fd = 0;
	pdfs.events = POLLIN;
	
	struct pollfd pdfs2;
	pdfs2.fd = 0;
	pdfs2.events = POLLIN;

	poll(&pdfs, 1, TIMEOUT);
	
	if (!(pdfs.revents & POLLIN))
	{
		printf("\n%s", buffer);
		return;
	}


	while ((index = get_number())!=0)
	{
		--index;
		if (index < 0 || index > t.size - 1)
		{
			printf("invalid line number: %ld\nline number: ", index + 1);
			continue;
		}

		size_t offset=0;
		size_t str_size=0;
		if (index == 0)
		{
			str_size = t.indices[index+1]+1;
			offset = 0;
		}
		else
		{
			str_size = t.indices[index+1]-t.indices[index];
			offset = t.indices[index]+1;
		}
		
		str = malloc(sizeof(char)*str_size);
		memcpy(str, (buffer+offset), str_size);
		
		if(!str)
		{
			perror("MEMORY ALLOCATION FAILED");
			return;
		}

		printf("%s",str);
		free(str);
	}
}

int main(int argc, char** argv)
{
	switch(argc)
	{
		case 2:
			break;
		case 3:
			break;
		default:
		return ERR_WRONG_ARGS;
			break;
	}
	printf("reading file: %s\n",argv[1]);
	
	off_t fd = file_open(argv[1]);
	off_t size = file_get_size(fd);
	char* buffer = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
	file_close(fd);
	table t = file_parse(buffer, size);
	file_get_string(buffer, size, t);
	free(t.indices);
	if (!munmap(buffer, size)){perror("munmap failed");}
	return 0;
}
