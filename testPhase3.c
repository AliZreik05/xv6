#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
volatile int i;
while(1)
{
for(i = 0 ; i < 50000000000;i++)
sleep(1);
}
exit();
}
