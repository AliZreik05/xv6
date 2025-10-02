#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

static int has_star(const char *s)
{
 for(; *s;s++)
{
if(*s == '*')
{
return 1;
}
}
return 0;
}

static int match_star(const char *pat, const char *s)
{
if(*pat == 0)
{
return *s == 0;
}
if(*pat == '*')
{
while(*s)
{
if(match_star(pat+1,s))
{
return 1;
}
s++;
}
return match_star(pat+1,s);
}
else
{
if(*s == 0)
{
return 0;
}
if(*pat == *s) 
{
return match_star(pat+1,s+1);
}
return 0;
}
}

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

static void print_entry(char *path)
{
struct stat st;
if(stat(path,&st) < 0)
{
printf(2,"ls: cannot stat %s\n",path);
return;
}
printf(1,"%s %d %d %d\n",fmtname(path),st.type,st.ino,st.size);
}

void
ls_plain(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

static void ls_glob(const char *dir, const char *pat)
{
int fd;
struct dirent de;
char name[DIRSIZ+1];
char path[512];

if((fd = open((char*)dir,0))<0)
{
printf(2,"ls:cannot open %s\n",dir);
return;
}

while(read(fd,&de,sizeof(de)) == sizeof(de))
{
if(de.inum == 0)
{
continue;
}

memmove(name,de.name,DIRSIZ);
name[DIRSIZ] = 0;

if(match_star(pat,name))
{
int dl = strlen(dir);
int nl = strlen(name);
int needslash = (dl == 0 || dir[dl-1] != '/');
if(dl +  (needslash ? 1 : 0) + nl + 1 > sizeof(path))
{
printf(2,"ls: path too long\n");
continue;
}

if(dl == 0)
{
path[0] = '.';
path[1]=0;
dl =1;
}
else
{
strcpy(path,dir);
}

if(path[dl-1] != '/')
{
path[dl] = '/';
path[dl+1] = 0;
}
{
int pl = strlen(path);
strcpy(path+pl,name);
}
print_entry(path);
}
}
close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2)
{
    ls_plain(".");
    exit();
  }
  for(i=1; i<argc; i++)
{
char *arg = argv[i];

if(has_star(arg))
{
char dir[512],pat[512];
char *slash = 0;
for(char *p = arg; *p; p++)
{
if(*p == '/')
{
slash = p;
}
}
if(slash)
{
int dlen = slash - arg;
if(dlen >= sizeof(dir))
{
printf(2,"ls: path too long\n");
continue;
}
memmove(dir,arg,dlen);
dir[dlen] =0;
if(strlen(slash+1) >= sizeof(pat))
{
printf(2,"ls: pattern too long\n");
continue;
}
strcpy(pat,slash+1);
}
else
{
strcpy(dir,".");
if(strlen(arg) >= sizeof(pat))
{
printf(2,"ls: pattern too long\n");
continue;
}
strcpy(pat,arg);
}
ls_glob(dir,pat);
}else
{
ls_plain(arg);
}
}
exit();
}
