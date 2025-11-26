#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
  struct procinfo ptable[64];
  int nproc = 64;

  if(getptable(nproc, ptable) == 0)
  {
    printf("Failed\n");
    exit(1);
  }

  printf("PID\tPPID\tSTATE\tSIZE\tNAME\n");
  for(int i = 0; i < nproc; i++)
  {
    if(ptable[i].pid == 0) continue;

    char *state;
    switch(ptable[i].state)
    {
      case UNUSED: state = "unused"; break;
      case USED: state = "used"; break;
      case SLEEPING: state = "sleep"; break;
      case RUNNABLE: state = "runable"; break;
      case RUNNING: state = "run"; break;
      case ZOMBIE: state = "zombie"; break;
      default: state = "???"; break;
    }

    printf("%d\t%d\t%s\t%lu\t%s\n",
    ptable[i].pid, ptable[i].ppid, state, ptable[i].sz, ptable[i].name);
  }

  exit(0);
}
