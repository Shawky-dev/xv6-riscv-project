#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if((argc == 2 && strcmp(argv[1], "?") == 0))
  {
    printf("Usage: sleep <ticks>\n");
    exit(0);
  }

  if (argc != 2)
  {
    printf("Error: Command takes one number\n");
    exit(1);
  }

  if (atoi(argv[1]) == 0)
  {
    printf("Error: Cant sleep for 0 ticks\n");
    exit(1);
  }

  int ticks = atoi(argv[1]);
  printf("Starting sleep for %d\n",ticks);
  sleep(ticks);
  printf("Sleep ended\n");
  exit(0);
}
