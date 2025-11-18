#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int fd;

    if ((fd = open(argv[1], 0)) < 0) {
        printf("find: cannot open %s\n", argv[1]);
        exit(0);
    } else {
        printf("find: file %s opened\n", argv[1]);
        close(fd);
        exit(0);
    }
}
