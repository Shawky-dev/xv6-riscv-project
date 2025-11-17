#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h" //has varible O_CREATE and O_RDWR

int main(int argc, char *argv[])
{
  int i;

  for (i = 1; i < argc; i++)
  {
    int fd = open(argv[i], O_CREATE | O_RDWR); // O_RDWR for future implmentation of timestamps if possible
    if (fd < 0)
    {
      printf("error creating file: %s\n", argv[i]);
    }
    else
    {
      close(fd);
    }
  }

  exit(0);
}
