#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

int main()
{
	char buf;
	while (read(STDIN_FILENO, &buf, 1) > 0)
	{
		buf = toupper(buf);
		write(STDOUT_FILENO, &buf, 1);
	}

	return 0;
}
