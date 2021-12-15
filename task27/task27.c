#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_MEM_ALLOC 1

int main(int argc, char** argv)
{
	if (argc!=2)
	{
		printf("usage: %s file\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	char* prefix = "grep \"^$\" ";
	char* postfix = " | wc -l";
	char* cmd = malloc(strlen(prefix)+strlen(argv[1])+strlen(postfix)+1);
	strcpy(cmd, prefix);
	strcat(cmd, argv[1]);
	strcat(cmd, postfix);
	FILE* p = popen(cmd, "r");
	free(cmd);

	if (!p)
	{
		perror("pipe open");
	}
	
	int count;
	fscanf(p, "%d", &count);
	printf("%d\n", count);

	if (pclose(p)) { perror("pipe close");}
	return EXIT_SUCCESS;
}

