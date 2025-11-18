// cpu.c
#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  printf(1, "cpu: starting infinite loop\n");
  volatile int x = 0;
  for(;;){
    x++;
  }
  exit();
}
