#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

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

int sys_ps(void){
  return get_ps();
}

#define MAX_CONTAINERS 16

struct container
{
  int allocated;
};

struct container container_list[MAX_CONTAINERS];

int sys_create_container(void){
  for(int i=1;i<MAX_CONTAINERS;i++){
    if (container_list[i].allocated==0){
      container_list[i].allocated=1;
      return i;
    }
  }
  return -1;
}

int sys_destroy_container(void){
  int cont_id;

  if(argint(0, &cont_id) < 0)
    return -1;

  if (cont_id>0 && container_list[cont_id].allocated){
    container_list[cont_id].allocated=0;
    // cleanup_processes(cont_id);
    return 0;
  }
  else
    return -2;
}

int sys_join_container(void){
  int cont_id;

  if(argint(0, &cont_id) < 0)
    return -1;

  if (cont_id>0 && container_list[cont_id].allocated){
    return proc_join_container(cont_id);
  }
  else
    return -1;
}

int sys_leave_container(void){
  return proc_leave_container();
}