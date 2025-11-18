
#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  if (fork()==0) 
  { 
    exec("spammer", 0);
    exit();
  }
  if (fork()==0) 
  {
    exec("spammer", 0);
    exit();
  }

  if (fork()== 0) 
  { 
    exec("testPhase3", 0);
    exit();
  }
  if (fork()== 0) 
  { 
  exec("testPhase3", 0); 
  exit();
  }

  while (1) { }
  exit();
}

