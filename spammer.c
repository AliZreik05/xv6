#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
volatile int i;
while(1)
{
for(i = 0 ; i < 10000000000000000;i++);
}
exit();
}
