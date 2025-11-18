#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "user/user.h"

#include "kernel/fs.h"
#include "kernel/stat.h"

void print_stat(struct stat *st) {
    printf("dev:   %d\n", st->dev);
    printf("ino:   %d\n", st->ino);
    printf("type:  %d\n", st->type);
    printf("nlink: %d\n", st->nlink);
    printf("size:  %lu\n", st->size);  // <<< fix
}


int main(int argc, char *argv[]) {
    int fd;
    char *path = argv[1];
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        printf("find: cannot open %s\n", path);
        exit(0);
    } else {
        printf("find: file %s opened\n", path);

        if (fstat(fd, &st) < 0) {
            printf("find: cannot stat %s\n", path);
            close(fd);
            exit(0);
        }

        print_stat(&st);
        close(fd);

        exit(0);
    }
}
