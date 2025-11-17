#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h" //has varible O_CREATE and O_RDWR

int main(int argc, char *argv[])
{
  int argv_f1 = 1;
  int argv_f2 = 2;
  // open source file
  int f1 = open(argv[argv_f1], O_RDONLY);
  if (f1 < 0)
  {
    printf("error opening source file: %s\n", argv[argv_f1]);
  }
  // open dest file
  int f2 = open(argv[argv_f2], O_CREATE | O_WRONLY | O_TRUNC);
  if (f2 < 0)
  {
    printf("error opening dest file: %s\n", argv[argv_f2]);
  }

  char buf[512];
  int n;

  while ((n = read(f1, buf, sizeof(buf))) > 0)
  {
    if (write(f2, buf, n) != n)
    {
      printf("error copying data to dest file: %s\n", argv[argv_f2]);
    }
  }

  close(f1);
  close(f2);
  exit(0);
}
