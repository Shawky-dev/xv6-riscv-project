#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Spin for approximately N ticks of CPU time
void burn_cpu(int target_ticks) {
  int start = uptime();
  volatile long counter = 0;

  while (uptime() - start < target_ticks) {
    for (int i = 0; i < 100000; i++) {
      counter++;
    }
  }
}

int main(int argc, char *argv[]) {
  int pid1, pid2, pid3;

  printf("=== FCFS Test ===\n");

  // Child1
  sleep(2);
  pid1 = fork();
  if (pid1 == 0) {
    printf("Child1 start (tick=%d)\n", uptime());
    burn_cpu(2);
    printf("Child1 sleep 30\n");
    sleep(30);
    printf("Child1 wake (tick=%d)\n", uptime());
    printf("Child1 burn 20\n");
    burn_cpu(20);
    printf("Child1 end (tick=%d)\n", uptime());
    exit(1);
  }

  sleep(10);

  // Child2
  sleep(2);
  pid2 = fork();
  if (pid2 == 0) {
    printf("Child2 start (tick=%d)\n", uptime());
    burn_cpu(5);
    printf("Child2 end (tick=%d)\n", uptime());
    exit(2);
  }

  sleep(10);

  // Child3
  sleep(2);
  pid3 = fork();
  if (pid3 == 0) {
    printf("Child3 start (tick=%d)\n", uptime());
    printf("Child3 burn 40\n");
    burn_cpu(40);
    printf("Child3 end (tick=%d)\n", uptime());
    exit(3);
  }

  int child_order[3];
  int idx = 0;

  for (int i = 0; i < 3; i++) {
    int status;
    int pid = wait(&status);
    printf("Exit: pid=%d (tick=%d)\n", pid, uptime());

    if (pid == pid1) child_order[idx++] = 1;
    else if (pid == pid2) child_order[idx++] = 2;
    else if (pid == pid3) child_order[idx++] = 3;
  }

  printf("Order: %d -> %d -> %d\n",
         child_order[0], child_order[1], child_order[2]);

  if (child_order[0] == 2 && child_order[1] == 1 && child_order[2] == 3) {
    printf("PASS: Expected 2 -> 1 -> 3\n");
  } else {
    printf("FAIL: Unexpected order\n");
  }

  exit(0);
}
