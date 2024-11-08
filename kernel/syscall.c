#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "syscall.h"
#include "syscallsCount.h"

// User code makes a system call with INT T_SYSCALL.
// System call number in %eax.
// Arguments on the stack, from the user call to the C
// library system call function. The saved user %esp points
// to a saved program counter, and then the first argument.

// Fetch the int at addr from the current process.
int
fetchint(uintp addr, int *ip)
{
  if(addr >= proc->sz || addr+sizeof(int) > proc->sz)
    return -1;
  *ip = *(int*)(addr);
  return 0;
}

int
fetchuintp(uintp addr, uintp *ip)
{
  if(addr >= proc->sz || addr+sizeof(uintp) > proc->sz)
    return -1;
  *ip = *(uintp*)(addr);
  return 0;
}

// Fetch the nul-terminated string at addr from the current process.
// Doesn't actually copy the string - just sets *pp to point at it.
// Returns length of string, not including nul.
int
fetchstr(uintp addr, char **pp)
{
  char *s, *ep;

  if(addr >= proc->sz)
    return -1;
  *pp = (char*)addr;
  ep = (char*)proc->sz;
  for(s = *pp; s < ep; s++)
    if(*s == 0)
      return s - *pp;
  return -1;
}

#if X64
// arguments passed in registers on x64
static uintp
fetcharg(int n)
{
  switch (n) {
  case 0: return proc->tf->rdi;
  case 1: return proc->tf->rsi;
  case 2: return proc->tf->rdx;
  case 3: return proc->tf->rcx;
  case 4: return proc->tf->r8;
  case 5: return proc->tf->r9;
  }
}

int
argint(int n, int *ip)
{
  *ip = fetcharg(n);
  return 0;
}

int
arguintp(int n, uintp *ip)
{
  *ip = fetcharg(n);
  return 0;
}
#else
// Fetch the nth 32-bit system call argument.
int
argint(int n, int *ip)
{
  return fetchint(proc->tf->esp + 4 + 4*n, ip);
}

int
arguintp(int n, uintp *ip)
{
  return fetchuintp(proc->tf->esp + sizeof(uintp) + sizeof(uintp)*n, ip);
}
#endif

// Fetch the nth word-sized system call argument as a pointer
// to a block of memory of size n bytes.  Check that the pointer
// lies within the process address space.
int
argptr(int n, char **pp, int size)
{
  uintp i;

  if(arguintp(n, &i) < 0)
    return -1;
  if(i >= proc->sz || i+size > proc->sz)
    return -1;
  *pp = (char*)i;
  return 0;
}

// Fetch the nth word-sized system call argument as a string pointer.
// Check that the pointer is valid and the string is nul-terminated.
// (There is no shared writable memory, so the string can't change
// between this check and being used by the kernel.)
int
argstr(int n, char **pp)
{
  uintp addr;
  if(arguintp(n, &addr) < 0)
    return -1;
  return fetchstr(addr, pp);
}

//pointers σε συναρτήσεις που βρίσκονται σε εξωτερικά αρχεία
extern int sys_chdir(void);
extern int sys_close(void);
extern int sys_dup(void);
extern int sys_exec(void);
extern int sys_exit(void);
extern int sys_fork(void);
extern int sys_fstat(void);
extern int sys_getpid(void);
extern int sys_kill(void);
extern int sys_link(void);
extern int sys_mkdir(void);
extern int sys_mknod(void);
extern int sys_open(void);
extern int sys_pipe(void);
extern int sys_read(void);
extern int sys_sbrk(void);
extern int sys_sleep(void);
extern int sys_unlink(void);
extern int sys_wait(void);
extern int sys_write(void);
extern int sys_uptime(void);
extern int sys_getpinfo(void);
extern int sys_getfavnum(void);
extern void sys_shutdown(void);
extern int sys_getcount(void);
extern int sys_killrandom(void);
extern void sys_settickets(void);

//Πίνακας που αντιστοιχεί το κάθε syscall number στον pointer της αντίστοιχης συνάρτησης
static int (*syscalls[])(void) = {
[SYS_fork]    sys_fork,
[SYS_exit]    sys_exit,
[SYS_wait]    sys_wait,
[SYS_pipe]    sys_pipe,
[SYS_read]    sys_read,
[SYS_kill]    sys_kill,
[SYS_exec]    sys_exec,
[SYS_fstat]   sys_fstat,
[SYS_chdir]   sys_chdir,
[SYS_dup]     sys_dup,
[SYS_getpid]  sys_getpid,
[SYS_sbrk]    sys_sbrk,
[SYS_sleep]   sys_sleep,
[SYS_uptime]  sys_uptime,
[SYS_open]    sys_open,
[SYS_write]   sys_write,
[SYS_mknod]   sys_mknod,
[SYS_unlink]  sys_unlink,
[SYS_link]    sys_link,
[SYS_mkdir]   sys_mkdir,
[SYS_close]   sys_close,
[SYS_getpinfo]    sys_getpinfo,
[SYS_getfavnum] sys_getfavnum,
[SYS_shutdown] sys_shutdown,
[SYS_getcount] sys_getcount,
[SYS_killrandom] sys_killrandom,
[SYS_settickets] sys_settickets
};

//Πίνακας που μετράει πόσες φορές εκτελέστηκε το κάθε syscall
int syscallsCount[SYSCALLCROWD] = {0};

void
syscall(void)
{
  int num;

  num = proc->tf->eax;
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    proc->tf->eax = syscalls[num]();
    syscallsCount[num-1] += 1;
  } else {
    cprintf("%d %s: unknown sys call %d\n",
            proc->pid, proc->name, num);
    proc->tf->eax = -1;
  }
}
