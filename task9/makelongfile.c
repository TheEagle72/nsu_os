#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define ERR_WRONG_ARGS 1
#define ERR_FILE_OPEN_FAILED 2

int main(int argc, char** argv)
{
	if (argc!=3)
	{
		printf("usage: %s *file name* number of lines\n", argv[0]);
		return 1;	
	}
	
	if (!isdigit(*argv[2]))
	{
		return ERR_WRONG_ARGS;
	}

	FILE* fp;
	fp = fopen(argv[1], "w");
	if (!fp)
	{
		return ERR_FILE_OPEN_FAILED;
	}

	for (int i = 0; i < strtol(argv[2], NULL, 10); ++i)	
	{
		fprintf(fp,"%d\n",i);
	}

	return 0;
}
