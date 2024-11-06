#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "syscallsCount.h"
#include "random.h"
#include "set.h"

extern struct ptable ptable;

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

int sys_getpinfo(void) 
{
    struct pstat *pstat;

    argptr(0, (void*)&pstat, sizeof(*pstat));

    if (pstat == 0) {
        return -1;
    }

    acquire(&ptable.lock);
    struct proc *p;
    int i;
    for (p = ptable.proc; p != &(ptable.proc[NPROC]); p++) 
    {
        i = p - ptable.proc;

        /* if (p->state == UNUSED) { */
        /*     pstat->inuse[i] = 0; */
        /* } else { */
        /*     pstat->inuse[i] = 1; */
        /* } */
        pstat->inuse[i] = p->inuse;
        pstat->pid[i] = p->pid;
        pstat->ticks[i] = p->ticks;
        pstat->tickets[i] = p->tickets;
    }
    release(&ptable.lock);
    return 0;
}

int
sys_getpid(void)
{
  return proc->pid;
}

uintp
sys_sbrk(void)
{
  uintp addr;
  uintp n;

  if(arguintp(0, &n) < 0)
    return -1;
  addr = proc->sz;
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
    if(proc->killed){
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

int sys_getfavnum(void){
  return 7;
}

void sys_shutdown(void){
  outw(0x604, 0x2000); //Γράφουμε στην διεύθυνση θύρας I/O 0x604 την τιμή 0x2000
  return;
}

int sys_getcount(void){
  int syscall;
  if(argint(0, &syscall) < 0) return -1; //Αν δεν υπάρχει όρισμα για το syscall επιστρέφουμε -1. Διαφορετικά το βάζουμε στην syscall
  cprintf("%d\n", syscall);
  cprintf("%d\n", syscallsCount[syscall-1]);
  return syscallsCount[syscall-1];
}

int sys_killrandom(void){
  LCG lcg;
  Set *set = createRoot();

  // Από sys_getpinfo
  acquire(&ptable.lock);
  struct proc *p;
  for (p = ptable.proc; p != &(ptable.proc[NPROC]); p++) { //Για κάθε διεργασία
      createNode(p->pid, set);
  }
  release(&ptable.lock);

  //Γεννήτρια ψευδοτυχαίων αριθμών
  lcg.m = set->size;
  lcg.a = sys_uptime()/(MAX(sys_getfavnum(), set->size));
  lcg.c = sys_getfavnum();
  lcg.state = 0;

  lcg_init(&lcg, sys_uptime());
  lcg_random(&lcg);
  
  cprintf("Killing PID: %d\n", getNodeAtPosition(set,lcg.state)->i);
  kill(getNodeAtPosition(set,lcg.state)->i);
  
  return 1;
}

void sys_settickets(void){
  int n;
  if (argint(0, &n) < 0) return;
  if (n < 1 || n > NPROCTICKETS) return;
  acquire(&ptable.lock);
  proc->tickets = n;
  release(&ptable.lock);
  return;
}