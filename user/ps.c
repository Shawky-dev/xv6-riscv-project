#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
  if((argc == 2 && strcmp(argv[1], "?") == 0))
  {
    printf("Desc: get pid & ppid, testing syscall getppid\n");
    exit(0);
  }

  int pid = getpid();
  int ppid = getppid();

  printf("PID: %d, PPID: %d\n", pid, ppid);

  exit(0);
}
