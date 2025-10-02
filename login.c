#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

struct userinfo {char * username; char* password;};
static struct userinfo usersList[] = 
{
{"root","admin"},
{"user","password",}
};

static int numberOfUsers = sizeof(usersList)/sizeof(usersList[0]);

static int streq(const char *a, const char *b)
{
for(; *a && *b ; a++,b++)
{
if (*a != *b)
{
return 0;
}
}
return *a== 0 && *b == 0;
}

static int auth(const char *u,const char *p)
{
for(int i = 0 ; i < numberOfUsers;i++)
{
if(streq(u,usersList[i].username) && streq(p,usersList[i].password))
{
return 1;
}
}
return 0;
}

int main(void)
{
if(open("console",O_RDWR < 0))
{
mknod("console" ,1,1);
open("console",O_RDWR);
}
dup(0);
dup(0);

char username[32],password[32];

printf(1,"Before you enter, you must log in:");
for(;;)
{
printf(1,"\nusername: ");
if(gets (username,sizeof(username))==0)
{
exit();
}
for(char *c = username; *c;c++)
{
if(*c == '\n')
{
*c = 0;
break;
}
}
printf(1,"password: ");
if(gets(password,sizeof(password)) == 0)
{
exit();
}
for(char *c = password; *c;c++)
{
if(*c == '\n')
{
*c = 0;
break;
}
}
if(auth(username,password))
{
printf(1,"Welcome!\n");
char *argv[] = {"sh",0};
exec("sh",argv);
exit();
}
else
{
printf(1,"invalid username or password");
sleep(50);
}
}
}

