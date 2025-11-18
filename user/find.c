#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* get_filename(char* path) {
    char* p = path + strlen(path);

    while(p >= path && *p != '/')
      p--;

    p++;
    return p;
}

void find(char* path, char* name) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        printf("find: cannot open %s\n", path);
        exit(0);
    }

    if (fstat(fd, &st) < 0) {
        printf("find: cannot stat %s\n", path);
        close(fd);
        exit(0);
    }

    switch(st.type) {
        case T_FILE:
            if (strcmp(get_filename(path), name) == 0)
                printf("%s\n", path);
            break;

        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) continue;
                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf, name);
            }
            break;
    }
    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc != 3 || (argc == 2 && strcmp(argv[1],"?") == 0)) {
        printf("Usage: find <directory> <filename>\n");
        exit(0);
    }

    find(argv[1], argv[2]);
    exit(0);
}
