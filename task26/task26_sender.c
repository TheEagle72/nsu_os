#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define ERR_PIPE_FAIL 1
#define ERR_MEM_ALLOC 2

char* concat(const char* str1, const char* str2)
{
	char* str = malloc(sizeof(char)*(strlen(str1)+strlen(str2)+1));
	strcpy(str, str1);
	strcat(str, str2);
	return str;
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("usage: %s cmd \n", argv[0]);
		return EXIT_FAILURE;
	}
	
	char* cmd = concat("./", argv[1]);
	FILE* p = popen(cmd, "w");
	free(cmd);
	if (p == NULL)
	{
		perror("pipe");
		return ERR_PIPE_FAIL;
	}
	printf("%s\n", argv[2]);
	fprintf(p, "%s\n", argv[2]);
	if (pclose(p)) { perror("pipe close"); }
}
