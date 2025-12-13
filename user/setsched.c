#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: setsched <sch_mode_number>\n");
    printf("  0 - Round Robin\n");
    printf("  1 - FCFS \n");
    printf("  2 - Priority\n");
    printf("  3 - My_FCFS\n");
    exit(1);
  }

  int sch_mode = atoi(argv[1]);
  char *mode_name;
  int result;

  switch(sch_mode) {
    case 0:
       result = setsched(SCHED_ROUND_ROBIN);
       mode_name = "Round Robin";
      break;
    case 1:
       result = setsched(SCHED_FCFS);
       mode_name = "FCFS";
      break;
    case 2:
       result = setsched(PRIORITY);
       mode_name = "Priority";
      break;
    case 3:
       result = setsched(SCHED_MY_FCFS);
       mode_name = "MY_FCFS";
      break;
    default:
      printf("Invalid sch_mode number: %d\n", sch_mode);
      printf("Valid sch_mode : 0, 1, 2, 3\n");
      exit(1);
  }

  if (result < 0) {
    printf("Error: Failed to set scheduler mode %d\n", sch_mode);
    exit(1);
  }

  printf("Scheduler set to: %s (mode %d)\n", mode_name, sch_mode);
  exit(0);
}
