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
    write(1, "error opening source file: ", 21);
    write(1, argv[argv_f1], strlen(argv[argv_f1]));
    write(1, "\n", 1);
  }
  // open dest file
  int f2 = open(argv[argv_f2], O_CREATE | O_WRONLY | O_TRUNC); // bitwise operation to do all 3, since 3 are binary values all 3 condtion will be tru
  if (f2 < 0)
  {
    write(1, "error opening dest file: ", 21);
    write(1, argv[argv_f2], strlen(argv[argv_f2]));
    write(1, "\n", 1);
  }

  char buf[512];
  int n;

  while ((n = read(f1, buf, sizeof(buf))) > 0)
  {
    if (write(f2, buf, n) != n)
    {
      write(1, "error coping data to  dest file: ", 21);
      write(1, argv[argv_f2], strlen(argv[argv_f2]));
      write(1, "\n", 1);
    }
  }

  close(f1);
  close(f2);
  exit(0);
}
