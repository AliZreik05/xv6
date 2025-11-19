 #include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BS 512   // read buffer size

// build out = prefix + "_" + 3-digit zero-padded idx (e.g., part_000, part_001, …)
// uses only strlen/strcpy (available in xv6 ulib)
static void mkname(char *out, const char *prefix, int idx){
  int i, pos;

  // out <- prefix
  strcpy(out, (char*)prefix);
  pos = strlen(out);

  // append '_'
  out[pos++] = '_';

  // prepare 3-digit zero-padded number in reverse
  char num[3];
  for(i = 2; i >= 0; i--){
    num[i] = '0' + (idx % 10);
    idx /= 10;
  }

  // append the 3 digits
  out[pos++] = num[0];
  out[pos++] = num[1];
  out[pos++] = num[2];

  out[pos] = 0;
}

int
main(int argc, char **argv)
{
  if(argc != 4){
    printf(2, "Usage: split file prefix chunk_size\n");
    exit();
  }

  int in = open(argv[1], O_RDONLY);
  if(in < 0){
    printf(2, "split: cannot open '%s'\n", argv[1]);
    exit();
  }

  int chunk = atoi(argv[3]);
  if(chunk <= 0){
    printf(2, "split: bad chunk_size\n");
    close(in);
    exit();
  }

  char buf[BS];
  int out = -1, left = chunk, idx = 0;
  char name[32];   // short; DIRSIZ is 14 for actual file entry, but xv6 truncates automatically

  int n;
  while((n = read(in, buf, sizeof buf)) > 0){
    int off = 0;
    while(off < n){
      if(out < 0){
        mkname(name, argv[2], idx++);
        out = open(name, O_CREATE | O_WRONLY);
        if(out < 0){
          printf(2, "split: cannot create '%s'\n", name);
          close(in);
          exit();
        }
        left = chunk;
      }

      int to = n - off;
      if(to > left) to = left;

      int w = write(out, buf + off, to);
      if(w < 0){
        printf(2, "split: write error\n");
        if(out >= 0) close(out);
        close(in);
        exit();
      }

      off  += w;
      left -= w;

      if(left == 0){
        close(out);
        out = -1;
      }
    }
  }

  if(out >= 0) close(out);
  close(in);
  exit();
}