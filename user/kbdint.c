#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    if((argc == 2 && strcmp(argv[1], "?") == 0))
  {
    printf("Desc: Count keyboard inputs, testing syscall kbdint\n");
    exit(0);
  }

  int count = kbdint();
  printf("Keyboard interrupts since boot: %d\n", count);

  exit(0);
}
