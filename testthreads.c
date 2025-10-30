#include "types.h"
#include "stat.h"
#include "user.h"
#include "mmu.h"
typedef int thread_t;

static int thread_create(thread_t *tid, void (*fn)(void*), void *arg) {
  void *stack = sbrk(PGSIZE);
  if((int)stack == -1) return -1;
  if(((uint)stack) % PGSIZE)
    stack = (void*)(((uint)stack + PGSIZE - 1) & ~(PGSIZE-1));
  int id = clone(fn, arg, stack);
printf(1,"clone ret=%d\n",id);
  if(id < 0) return -1;
  if(tid) *tid = id;
  return 0;
}

static int thread_join(thread_t *tid /*optional*/) {
  void *ustack = 0;
  int got = join(&ustack);
  if(got < 0) return -1;
  if(tid) *tid = got;
  return 0;
}

static void thread_exit_user(void) {
  threadexit();
}

static volatile uint lock = 0;
static void acquire(void){ while(__sync_lock_test_and_set(&lock,1)!=0); }
static void release(void){ __sync_lock_release(&lock); }

volatile int counter = 0;

void worker(void *arg){
  int n = (int)arg;
  for(int i=0;i<n;i++){
    acquire();
    counter++;
    release();
  }
  threadexit();
}

int
main(void)
{
  int threads = 4;
  int per = 100000;
  thread_t tid;

  for(int i=0;i<threads;i++)
    thread_create(&tid, worker, (void*)per);

  for(int i=0;i<threads;i++)
    thread_join(0);

  printf(1, "counter=%d expected=%d\n", counter, threads*per);
  exit();
}
