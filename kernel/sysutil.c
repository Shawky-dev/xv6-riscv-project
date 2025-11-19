#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

//kernel logic for rand syscall
//LCG equation: Xn+1 = (a * Xn + c) mod m
//the first x is the seed
//we can get a seed mtime register or ticks
extern uint ticks;

static unsigned long rand_seed = 1;

uint64
sys_myrand(void)
{
  //initialize seed if it's the first call
  if(rand_seed == 1)
  {
    rand_seed = ticks;
  }

  //LCG parameters
  unsigned long a = 1664525;
  unsigned long c = 1013904223;
  unsigned long m = 4294967296; //2^32

  //update seed using LCG formula
  rand_seed = (a * rand_seed + c) % m;

  return rand_seed;
}
