#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

void tree(char *path, int level) {
    int fd;
    struct dirent de;
    struct stat st;
    char buf[512], *p;

    if((fd = open(path, 0)) < 0){
        printf(1, "tree: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        printf(1, "tree: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if(st.type != T_DIR){
        printf(1, "%s\n", path);
        close(fd);
        return;
    }

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0) continue;
        if(!strcmp(de.name, ".") || !strcmp(de.name, "..")) continue;

        for(int i = 0; i < level; i++)
            printf(1, "  ");  // Indentation for directory depth

        printf(1, "|-- %s\n", de.name);  // Print directory or file name

        memmove(buf, path, strlen(path));
        p = buf + strlen(path);
        *p++ = '/';
        memmove(p, de.name, strlen(de.name));
        p[strlen(de.name)] = 0;

        stat(buf, &st);
        if(st.type == T_DIR)
            tree(buf, level + 1);  // Recurse into subdirectories
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if(argc < 2){
        tree(".", 0);  // Default to current directory
    } else {
        tree(argv[1], 0);  // Print tree of provided directory
    }
    exit();  // Exit the program
}
