#include <stdio.h>
#include <unistd.h>
#include <errno.h>

void func(const char* file_path)
{
FILE* F = fopen(file_path,"r");
if (F==NULL)
{
perror("Error");
return;
}
printf("file opened sucessfully\n");
fclose(F);
}

int main(int argc, char** argv)
{
const char* file_path = "data";
if (argc==1)
{
printf("need file name\n");
return 1;
}

uid_t real = getuid();
uid_t eff = geteuid();
printf("real id: %d\neffective id: %d\n", real, eff);
func(argv[1]);
seteuid(real);
printf("seteuid\n");
real = getuid();
eff = geteuid();
printf("real id: %d\neffective id: %d\n", real, eff);
func(argv[1]);
return 0;
}
