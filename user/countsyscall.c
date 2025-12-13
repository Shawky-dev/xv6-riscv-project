#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
  if((argc == 2 && strcmp(argv[1], "?") == 0))
  {
    printf("Desc: Count System calls, testing syscall countsyscall\n");
    exit(0);
  }


  int count = countsyscall();

  printf("System calls called since boot %d\n", count);

  exit(0);
}
