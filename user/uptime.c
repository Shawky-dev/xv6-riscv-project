#include "kernel/types.h"
#include "user/user.h"

int main()
{
  int ticks = uptime();
  int seconds = ticks / 10;
  printf("System Uptime since boot: Ticks:%d Seconds:%d\n", ticks, seconds); // a tick = 1 tenth of a second as it can be seen in a comment inside trap.c

  exit(0);
}
