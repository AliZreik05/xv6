#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_clone(void)
{
  int f,a,s;
  if(argint(0,&f) < 0) return -1;
  if(argint(1,&a) < 0) return -1;
  if(argint(2,&s) < 0) return -1;
  return clone((void(*)(void*))f, (void*)a, (void*)s);
}

int
sys_join(void)
{
  void **p;
  if(argptr(0,(void*)&p,sizeof(p)) < 0) return -1;
  return join(p);
}

int
sys_threadexit(void)
{
  threadexit();
  return 0;
}


int sys_getsysteminfo(void)
{
char *ubuf;
int info[3];

if(argptr(0,&ubuf,sizeof(info)) < 0)
{
return -1;
}
info[0] = numberOfProcesses();
info[1] = FreeMemory();
info[2] = Uptime();

memmove(ubuf,info,sizeof(info));
return 0;
}

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
extern uint ticks;
extern struct spinlock ticklock;

int Uptime(void)
{
uint ticksnb;
acquire(&tickslock);
ticksnb = ticks;
release(&tickslock);
return (int)ticksnb;
}
