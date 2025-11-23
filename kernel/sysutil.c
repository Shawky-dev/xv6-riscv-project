#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"
#include "defs.h"


extern int syscall_count;

int
sys_countsyscall(void)
{
  return syscall_count;
}
