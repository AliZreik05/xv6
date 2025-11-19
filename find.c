 #include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

// simple substring match (since xv6 has no strstr)
static int contains(const char *s, const char *pat){
  int i, j;
  if(!pat[0]) return 1;
  for(i = 0; s[i]; i++){
    for(j = 0; pat[j] && s[i+j] == pat[j]; j++) ;
    if(pat[j] == 0) return 1;
  }
  return 0;
}

static void walk(char *path, char *pat){
  int fd; struct stat st; struct dirent de;

  if((fd = open(path, 0)) < 0) return;
  if(fstat(fd, &st) < 0){ close(fd); return; }

  if(st.type == T_FILE){
    // match on basename
    char *p = path + strlen(path);
    while(p > path && *(p-1) != '/') p--;
    if(contains(p, pat)) printf(1, "%s\n", path);
  } else if(st.type == T_DIR){
    char buf[512]; char *p;
    int n = strlen(path);
    memmove(buf, path, n);
    buf[n] = '/';
    p = buf + n + 1;

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0) continue;
      if(!strcmp(de.name, ".") || !strcmp(de.name, "..")) continue;

      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      if(stat(buf, &st) < 0) continue;

      if(st.type == T_DIR){
        walk(buf, pat);
      } else {
        if(contains(de.name, pat)) printf(1, "%s\n", buf);
      }
    }
  }
  close(fd);
}

int
main(int argc, char **argv){
  if(argc != 3){
    printf(2, "Usage: find start_dir pattern\n");
    exit();
  }
  walk(argv[1], argv[2]);
  exit();
}