#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "user/user.h"

#define DEFAULT_LINES 10
#define MAX_LINE 512

void tail(int fd, int nlines) {
    char buf[MAX_LINE];
    char *lines[nlines];
    int i, count = 0, n;

    // Initialize circular buffer
    for (i = 0; i < nlines; i++) {
        lines[i] = 0;
    }

    // Read file line by line
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        char *p = buf;
        char *start = buf;

        while (p < buf + n) {
            if (*p == '\n' || p == buf + n - 1) {
                // End of line found
                int len = p - start + 1;
                if (*p != '\n') len++;  // Handle last line without newline

                char *line = malloc(len + 1);
                if (line == 0) {
                    printf("tail: out of memory\n");
                    exit(1);
                }

                memmove(line, start, len);
                line[len] = 0;

                // Free old line and store new one
                if (lines[count % nlines]) {
                    free(lines[count % nlines]);
                }
                lines[count % nlines] = line;
                count++;

                start = p + 1;
            }
            p++;
        }
    }

    // Print the last nlines using printf
    int start_idx = count > nlines ? count % nlines : 0;
    int num_to_print = count < nlines ? count : nlines;

    for (i = 0; i < num_to_print; i++) {
        printf("%s", lines[(start_idx + i) % nlines]);
        free(lines[(start_idx + i) % nlines]);
    }

    if (n < 0) {
        printf("tail: read error\n");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    int fd, nlines = DEFAULT_LINES;
    int i;

    // Parse -n option for number of lines
    if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'n') {
        if (argc < 3) {
            printf("Usage: tail [-n lines] [file ...]\n");
            exit(1);
        }
        nlines = atoi(argv[2]);
        if (nlines <= 0) nlines = DEFAULT_LINES;

        // Shift arguments
        argv += 2;
        argc -= 2;
    }

    // Handle no arguments or just -n without file
    if (argc <= 1) {
        tail(0, nlines);  // Read from stdin
        exit(0);
    }

    // Process each file
    for (i = 1; i < argc; i++) {
        if ((fd = open(argv[i], O_RDONLY)) < 0) {
            printf("tail: cannot open %s\n", argv[i]);
            exit(1);
        }
        tail(fd, nlines);
        close(fd);
    }
    exit(0);
}
