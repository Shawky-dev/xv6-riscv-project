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

int
sys_shutdown(void)
{
    // Trigger QEMU shutdown via memory-mapped I/O
    *(uint32*)0x100000 = 0x5555;
    return 0;  // Not reached
}
