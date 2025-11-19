#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

// Minimal strstr implementation to avoid standard library conflicts
char *my_strstr(const char *haystack, const char *needle) {
    if (!*needle)
        return (char*)haystack;

    for (; *haystack; haystack++) {
        const char *h = haystack;
        const char *n = needle;
        while (*h && *n && *h == *n) {
            h++;
            n++;
        }
        if (!*n)
            return (char*)haystack;
    }
    return 0;
}

#define MAXLINE 512

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf(2, "Usage: search <file> <keyword>\n");
        exit();
    }

    char *filename = argv[1];
    char *keyword = argv[2];

    int fd = open(filename, 0);
    if (fd < 0) {
        printf(2, "search: cannot open %s\n", filename);
        exit();
    }

    char buf[MAXLINE];
    int n;
    char line[MAXLINE];
    int line_len = 0;

    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < n; i++) {
            if (buf[i] == '\n') {
                line[line_len] = 0; // terminate the line
                if (my_strstr(line, keyword)) {
                    printf(1, "%s\n", line);
                }
                line_len = 0; // reset for next line
            } else {
                if (line_len < MAXLINE - 1) {
                    line[line_len++] = buf[i];
                }
            }
        }
    }

    // handle last line if it doesn't end with '\n'
    if (line_len > 0) {
        line[line_len] = 0;
        if (my_strstr(line, keyword)) {
            printf(1, "%s\n", line);
        }
    }

    close(fd);
    exit();
}
