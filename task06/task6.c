#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/poll.h>

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
	close(fd);
}

off_t get_number(void)
{
	off_t line_number;
	while (scanf("%ld", &line_number)!=1)
	{
		while (getchar()!='\n'); //clear buffer
		printf("invalid line number\nenter line number: ");
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
	off_t str_size = t.indices[index+1]-t.indices[index];
	fflush(stdout);

	struct pollfd pdfs;
	pdfs.fd = 0;
	pdfs.events = POLLIN;

	poll(&pdfs, 1, TIMEOUT);
	
	if (!(pdfs.revents & POLLIN))
	{
		printf("\n%s", buffer);
		return;
	}

	while ((index = get_number()) != 0)
	{
		--index;
		if (index < 0 || index > t.size - 1)
		{
			printf("invalid line number: %ld\n", index + 1);
			continue;
		}

		if (index == t.size-1)
		{
			
			str_size = buffer_size-t.indices[index]-2;
			str = malloc(sizeof(char)*str_size);
			memcpy(str, (buffer+t.indices[index]+1), str_size);
		}
		else
		{	
			str_size = t.indices[index+1]-t.indices[index];
			str = malloc(sizeof(char)*str_size);
			memcpy(str, (buffer+t.indices[index]), str_size);
		}
		if(!str)
		{
			perror("MEMORY ALLOCATION FAILED");
			return;
		}
		printf("%ld: %s\n", index+1, str);
		free(str);
	}
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		return ERR_WRONG_ARGS; 
	}
	printf("reading file: %s\n",argv[1]);
	
	off_t fd = file_open(argv[1]);
	off_t size = file_get_size(fd);
	char* buffer = file_read(fd, size);
	file_close(fd);
	table t = file_parse(buffer, size);
	file_get_string(buffer, size, t);
	free(t.indices);
	free(buffer);
	return 0;
}
