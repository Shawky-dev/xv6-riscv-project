#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"
#include "defs.h"
#include "datetime.h"


extern uint ticks;

static unsigned long rand_seed = 1;

extern int syscall_count;

int sys_countsyscall(void)
{
  return syscall_count;
}

int sys_shutdown(void)
{
  // Trigger QEMU shutdown via memory-mapped I/O
  *(uint32 *)0x100000 = 0x5555;
  return 0; // Not reached
}


// kernel logic for rand syscall
// LCG equation: Xn+1 = (a * Xn + c) mod m
// the first x is the seed
// we can get a seed mtime register or ticks
uint64
sys_myrand(void)
{
  // initialize seed if it's the first call
  if (rand_seed == 1)
  {
    rand_seed = ticks;
  }

  // LCG parameters
  unsigned long a = 1664525;
  unsigned long c = 1013904223;
  unsigned long m = 4294967296; // 2^32

  // update seed using LCG formula
  rand_seed = (a * rand_seed + c) % m;

  return rand_seed;
}

// kernel logic for datetime syscall

// converts unix timestamp to real date time
static int days_in_month(int month, int year)
{
  switch (month)
  {
  case 1:
    return 31;
  // february check for leap year
  case 2:
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
  case 3:
    return 31;
  case 4:
    return 30;
  case 5:
    return 31;
  case 6:
    return 30;
  case 7:
    return 31;
  case 8:
    return 31;
  case 9:
    return 30;
  case 10:
    return 31;
  case 11:
    return 30;
  case 12:
    return 31;
  }
  return 30;
}

void unix_to_realdt(uint64 ts, struct real_date_time *dt)
{
  // raw seconds conversion
  uint64 sec = ts;

  dt->second = sec % 60;
  sec /= 60;
  dt->minute = sec % 60;
  sec /= 60;
  dt->hour = sec % 24;
  sec /= 24;

  int year = 1970;
  while (1)
  {
    int days = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 366 : 365;
    if (sec >= days)
    {
      sec -= days;
      year++;
    }
    else
      break;
  }
  dt->year = year;

  int month = 1;
  while (1)
  {
    int dim = days_in_month(month, year);
    if (sec >= dim)
    {
      sec -= dim;
      month++;
    }
    else
      break;
  }
  dt->month = month;
  dt->day = sec + 1;
}

uint64
sys_datetime(void)
{
  uint64 user_addr;
  struct real_date_time dt;

  // get user pointer
  argaddr(0, &user_addr);

  // get current mtime
  uint64 mtime = r_time();

  // covert ticks to seconds
  uint64 seconds = mtime / CLOCKFREQ;

  uint64 ts = BOOT_EPOCH + seconds;

  // convert UNIX timestamp → calendar date
  unix_to_realdt(ts, &dt);

  // copy result to user space
  if (copyout(myproc()->pagetable, user_addr, (char *)&dt, sizeof(dt)) < 0)
  {
    return -1;
  }

  return 0;
}

extern int kbd_intr_count;

uint64
sys_kbdint(void)
{
  return kbd_intr_count;
}
