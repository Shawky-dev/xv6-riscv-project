#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Spin for approximately N ticks of CPU time
void burn_cpu(int target_ticks) {
  int start = uptime();
  volatile long counter = 0;

  while (uptime() - start < target_ticks) {
    // Just spin - this forces the process to use CPU
    for (int i = 0; i < 100000; i++) {
      counter++;
    }
  }
}

// Test 0: Basic FCFS - processes created in order, run in order
void test0() {
  printf("\n=== Test 0: Basic FCFS (No Sleep) ===\n");
  printf("All processes ready immediately, should run in creation order\n\n");

  int pids[3];
  int child_order[3];
  int order_idx = 0;

  // Create 3 children with delays to ensure different creation times
  for (int i = 0; i < 3; i++) {
    sleep(2);

    int pid = fork();
    if (pid == 0) {
      int start = uptime();
      printf("[Child%d pid=%d ctime=%d] Created\n", i+1, getpid(), start);

      // All do similar work
      burn_cpu(10);

      int end = uptime();
      printf("[Child%d pid=%d] FINISHED at tick %d\n", i+1, getpid(), end);
      exit(i+1);
    }
    pids[i] = pid;
  }

  // Wait for all children
  for (int i = 0; i < 3; i++) {
    int status;
    int pid = wait(&status);

    for (int j = 0; j < 3; j++) {
      if (pid == pids[j]) {
        child_order[order_idx++] = j+1;
        break;
      }
    }
  }

  printf("\n========================================\n");
  printf("Completion order: Child%d -> Child%d -> Child%d\n",
         child_order[0], child_order[1], child_order[2]);

  if (child_order[0] == 1 && child_order[1] == 2 && child_order[2] == 3) {
    printf("✓✓✓ PASS: FCFS order 1 -> 2 -> 3 ✓✓✓\n");
  } else {
    printf("✗✗✗ FAIL: Expected 1 -> 2 -> 3 ✗✗✗\n");
  }
  printf("========================================\n");
}

// Test 1: Sleep/Wake - first process sleeps, later process should not overtake
void test1() {
  printf("\n=== Test 1: Sleep/Wake Preemption ===\n");
  printf("Testing: Process created first should run first even after sleeping\n\n");

  int pid1, pid2, pid3;

  // Child1: Created first
  sleep(2);
  pid1 = fork();
  if (pid1 == 0) {
    int start = uptime();
    printf("[Child1 pid=%d ctime=%d] Created FIRST\n", getpid(), start);

    burn_cpu(2);

    printf("[Child1 pid=%d] Going to sleep for 30 ticks\n", getpid());
    sleep(30);

    int wake = uptime();
    printf("[Child1 pid=%d] Woke at tick %d\n", getpid(), wake);

    printf("[Child1 pid=%d] Burning 20 CPU ticks...\n", getpid());
    burn_cpu(20);

    int end = uptime();
    printf("[Child1 pid=%d] FINISHED at tick %d\n", getpid(), end);
    exit(1);
  }

  // Wait for Child1 to sleep
  sleep(10);

  // Child2: Created second, finishes quickly
  sleep(2);
  pid2 = fork();
  if (pid2 == 0) {
    int start = uptime();
    printf("[Child2 pid=%d ctime=%d] Created SECOND\n", getpid(), start);

    burn_cpu(5);

    int end = uptime();
    printf("[Child2 pid=%d] FINISHED at tick %d\n", getpid(), end);
    exit(2);
  }

  // Wait for Child2 to finish
  sleep(10);

  // Child3: Created third, does long work
  sleep(2);
  pid3 = fork();
  if (pid3 == 0) {
    int start = uptime();
    printf("[Child3 pid=%d ctime=%d] Created THIRD\n", getpid(), start);
    printf("[Child3 pid=%d] Burning 40 CPU ticks (Child1 will wake during this)...\n",
           getpid());

    burn_cpu(40);

    int end = uptime();
    printf("[Child3 pid=%d] FINISHED at tick %d\n", getpid(), end);
    exit(3);
  }

  // Wait for all children
  int child_order[3];
  int order_idx = 0;

  for (int i = 0; i < 3; i++) {
    int status;
    int pid = wait(&status);

    if (pid == pid1) child_order[order_idx++] = 1;
    else if (pid == pid2) child_order[order_idx++] = 2;
    else if (pid == pid3) child_order[order_idx++] = 3;
  }

  printf("\n========================================\n");
  printf("Completion order: Child%d -> Child%d -> Child%d\n",
         child_order[0], child_order[1], child_order[2]);

  if (child_order[0] == 2 && child_order[1] == 1 && child_order[2] == 3) {
    printf("✓✓✓ PASS: Correct FCFS order 2 -> 1 -> 3 ✓✓✓\n");
    printf("Child1 (created first) preempted Child3 after waking!\n");
  } else if (child_order[0] == 2 && child_order[1] == 3 && child_order[2] == 1) {
    printf("✗✗✗ FAIL: Wrong order 2 -> 3 -> 1 ✗✗✗\n");
    printf("Child3 finished before Child1, violating FCFS!\n");
  } else {
    printf("? Unexpected order - check timing\n");
  }
  printf("========================================\n");
}

// Test 2: Multiple sleeps - complex scenario with multiple wake events
void test2() {
  printf("\n=== Test 2: Multiple Sleep/Wake Events ===\n");
  printf("Testing: Multiple processes sleeping and waking\n\n");

  int pid1, pid2, pid3;

  // Child1: Sleeps twice
  sleep(2);
  pid1 = fork();
  if (pid1 == 0) {
    int start = uptime();
    printf("[Child1 pid=%d ctime=%d] Created FIRST\n", getpid(), start);

    burn_cpu(3);
    printf("[Child1 pid=%d] First sleep (15 ticks)\n", getpid());
    sleep(15);

    printf("[Child1 pid=%d] Woke from first sleep, working...\n", getpid());
    burn_cpu(5);

    printf("[Child1 pid=%d] Second sleep (15 ticks)\n", getpid());
    sleep(15);

    printf("[Child1 pid=%d] Woke from second sleep, finishing...\n", getpid());
    burn_cpu(10);

    int end = uptime();
    printf("[Child1 pid=%d] FINISHED at tick %d\n", getpid(), end);
    exit(1);
  }

  // Child2: Short sleep
  sleep(5);
  pid2 = fork();
  if (pid2 == 0) {
    int start = uptime();
    printf("[Child2 pid=%d ctime=%d] Created SECOND\n", getpid(), start);

    burn_cpu(3);
    printf("[Child2 pid=%d] Sleeping (10 ticks)\n", getpid());
    sleep(10);

    printf("[Child2 pid=%d] Woke up, working...\n", getpid());
    burn_cpu(8);

    int end = uptime();
    printf("[Child2 pid=%d] FINISHED at tick %d\n", getpid(), end);
    exit(2);
  }

  // Child3: Long continuous work
  sleep(10);
  pid3 = fork();
  if (pid3 == 0) {
    int start = uptime();
    printf("[Child3 pid=%d ctime=%d] Created THIRD\n", getpid(), start);
    printf("[Child3 pid=%d] Long continuous work (50 ticks)...\n", getpid());

    burn_cpu(50);

    int end = uptime();
    printf("[Child3 pid=%d] FINISHED at tick %d\n", getpid(), end);
    exit(3);
  }

  // Wait for all children
  int child_order[3];
  int order_idx = 0;

  for (int i = 0; i < 3; i++) {
    int status;
    int pid = wait(&status);

    if (pid == pid1) child_order[order_idx++] = 1;
    else if (pid == pid2) child_order[order_idx++] = 2;
    else if (pid == pid3) child_order[order_idx++] = 3;
  }

  printf("\n========================================\n");
  printf("Completion order: Child%d -> Child%d -> Child%d\n",
         child_order[0], child_order[1], child_order[2]);

  // With FCFS and the timing above:
  // Child1 (earliest creation) should get CPU whenever it's ready
  // Child2 (second earliest) should get CPU before Child3
  // Expected: Some permutation where earlier processes don't starve

  // The key test: Child1 should NOT finish last (it was created first)
  if (child_order[2] != 1) {
    printf("✓✓✓ PASS: Child1 didn't finish last ✓✓✓\n");
    printf("FCFS respects creation order across multiple wake events\n");
  } else {
    printf("✗✗✗ FAIL: Child1 finished last ✗✗✗\n");
    printf("Child1 was created first but finished last!\n");
  }
  printf("========================================\n");
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: minimal_test <test_number>\n");
    printf("  0 - Basic FCFS (no sleep)\n");
    printf("  1 - Sleep/Wake preemption\n");
    printf("  2 - Multiple sleep/wake events\n");
    exit(1);
  }

  int test_num = atoi(argv[1]);

  switch(test_num) {
    case 0:
      test0();
      break;
    case 1:
      test1();
      break;
    case 2:
      test2();
      break;
    default:
      printf("Invalid test number: %d\n", test_num);
      printf("Valid tests: 0, 1, 2\n");
      exit(1);
  }

  exit(0);
}
