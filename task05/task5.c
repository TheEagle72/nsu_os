#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
//open
//close
//lseek
//read

#define ERR_SUCCESS 0
#define ERR_WRONG_ARGS 1
#define ERR_FILE_OPEN_FAILED 2
#define ERR_INVALID_SIZE 3

typedef int error_type;

off_t file_get_size(int fd)
{
	if (fd==-1)
	{
		return 0;
	}

	off_t file_size = lseek(fd, 0L, SEEK_END);
	lseek(fd, 0L, SEEK_SET);
	return file_size;
}

int file_open(char* path)
{
	
	//trying to open file
	int file_descriptor = open(path, O_RDONLY); // file descriptor
	if (file_descriptor == -1)
	{
		perror("file can't be opened");
		return -1;
	}

	return file_descriptor;
}

char* file_read(int fd, size_t size)
{
	if (size < 0)
	{
		return NULL;
	}

	char* buffer = malloc(sizeof(char)*size);
	read(fd, buffer, size);
	return buffer;
}

int file_close(int fd)
{
	close(fd);
}

int get_number()
{
	int line_number;
	while (scanf("%d", &line_number)!=1)
	{
		while (getchar()!='\n'); //clear buffer
		printf("invalid line number: %d\n", line_number);
	}
	return line_number;
}

#define DEFAULT_COUNT 8
#define INCREMENT_COUNT 8

typedef struct table
{
	size_t size;
	off_t* indices;

} table;


table file_parse(char* buffer, size_t size)
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

void file_get_string(char* buffer, off_t buffer_size, table t)
{
	char* str = NULL;
	int index = 0;
	off_t str_size = t.indices[index+1]-t.indices[index];
	while ((index = get_number()) != 0)
	{
		--index;
		if (index < 0 || index > t.size - 1)
		{
			printf("invalid line number: %d\n", index + 1);
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
		printf("%s\n", str);
		free(str);
	}
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("usage: %s file\n",argv[0]);
		return ERR_WRONG_ARGS; 
	}
	
	int fd = file_open(argv[1]);
	if (fd==-1)
	{
		return 1;
	}
	off_t size = file_get_size(fd);
	char* buffer = file_read(fd, size);
	file_close(fd);
	table t = file_parse(buffer, size);
	file_get_string(buffer, size, t);
	free(t.indices);
	free(buffer);
	return 0;
}
