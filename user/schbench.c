#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[]) {
  int pid;
  int k, nprocess = 10;
  int z, steps = 1000000;
  char buffer_src[1024], buffer_dst[1024];
  int start_time, end_time;
  int termination_times[10];

  printf("Scheduler test starting at tick %d\n", uptime());
  start_time = uptime();

  for (k = 0; k < nprocess; k++) {
    // ensure different creation times (proc->ctime)
    // needed for properly testing FCFS scheduling
    sleep(2);

    pid = fork();
    if (pid < 0) {
      printf("%d failed in fork!\n", getpid());
      exit(0);
    }
    else if (pid == 0) {
      // child
      int child_start = uptime();
      // setpriority(getpid());
      printf("[pid=%d] created at tick %d, priority=%d\n", getpid(), child_start, getpriority());

      for (z = 0; z < steps; z += 1) {
         memmove(buffer_dst, buffer_src, 1024);
         memmove(buffer_src, buffer_dst, 1024);
      }

      int child_end = uptime();
      printf("[pid=%d] finished at tick %d (duration: %d)\n",
             getpid(), child_end, child_end - child_start);
      exit(0);
    }
  }

  // Wait for all children and collect termination times
  for (k = 0; k < nprocess; k++) {
    pid = wait(0);
    termination_times[k] = uptime();
    printf("[pid=%d] terminated at tick %d\n", pid, termination_times[k]);
  }

  // 🔹 NEW: ask kernel to print scheduler statistics
  schedstats();

  end_time = uptime();
  printf("\n=== Scheduling Metrics ===\n");
  printf("Total execution time: %d ticks\n", end_time - start_time);

  exit(0);
}
