#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
int info[3];
if(getsysteminfo(info) < 0 )
{
printf(1,"failed to execute command: getsysteminfo");
exit();
}
printf(1,"Number of Porcesses: %d\n",info[0]);
printf(1,"Free Memory: %d\n",info[1]);
printf(1,"Uptime: %d\n",info[2]);
exit();
}
