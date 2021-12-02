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

	FILE* p = popen("wc -l", "w");
	if (!p)
	{
		perror("pipe open");
	}
	
	FILE* f = fopen(argv[1], "r");
	char* buf = malloc(BUFSIZ);
	if (!buf)
	{
		perror("malloc");
		return ERR_MEM_ALLOC;
	}
	while (fgets(buf, BUFSIZ, f))
	{
		if (strlen(buf) != 1)
		{
			fprintf(p, "%s", buf);
		}
	}

	//grep '^$' data | wc -l

	if (pclose(p)) { perror("pipe close");}
	return EXIT_SUCCESS;
}

