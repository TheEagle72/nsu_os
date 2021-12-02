#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 101

int main()
{
	list* lst = NULL;
	list_create(&lst);
	do
	{
		char* buff = malloc(sizeof(char) * MAX_SIZE);
		if (!buff)
		{
			perror("memory was not allocated");
			return ERR_MEM_ALLOC;
		}

		fgets(buff, MAX_SIZE, stdin);
		if (*buff == '.')
		{
			break;
		}

		//list_prepend(&lst, buff, strlen(buff) + 1);
		list_append(lst, buff, strlen(buff) + 1);
		free(buff);

	} while (1);

	list_print(lst);
	list_delete(lst);
	return 0;
}

