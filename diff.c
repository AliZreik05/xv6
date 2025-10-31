#include "types.h"
#include "user.h"
#include "stat.h"

#define BUFSIZE 512  // Buffer size for reading the file
#define O_RDONLY 0  // Open the file for reading only

// Function to compare two files
int diff(int fd1, int fd2) {
    char buf1[BUFSIZE], buf2[BUFSIZE];
    int n1, n2, line = 1;

    while(1) {
        // Read a chunk from both files
        n1 = read(fd1, buf1, BUFSIZE);
        n2 = read(fd2, buf2, BUFSIZE);

        // If both files have ended
        if(n1 == 0 && n2 == 0) {
            return 0;  // No difference found
        }

        // If the lengths are different
        if(n1 != n2) {
            printf(1, "Files differ at line %d\n", line);
            return -1;
        }

        // Compare the contents byte by byte
        for(int i = 0; i < n1; i++) {
            if(buf1[i] != buf2[i]) {
                printf(1, "Files differ at line %d\n", line);
                return -1;
            }
        }
        line++;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf(1, "Usage: diff <file1> <file2>\n");
        exit();
    }

    int fd1, fd2;

    // Open both files
    fd1 = open(argv[1], O_RDONLY);
    if(fd1 < 0) {
        printf(1, "diff: cannot open %s\n", argv[1]);
        exit();
    }

    fd2 = open(argv[2], O_RDONLY);
    if(fd2 < 0) {
        printf(1, "diff: cannot open %s\n", argv[2]);
        exit();
    }

    // Compare the two files
    if(diff(fd1, fd2) == 0) {
        printf(1, "Files are the same\n");
    }

    // Close the files
    close(fd1);
    close(fd2);

    exit();
}
