#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h" //has varible O_CREATE and O_RDWR


int
main(int argc, char *argv[])
{
  int i;

  for(i = 1; i < argc; i++){
    int fd = open(argv[i],O_CREATE | O_RDWR);
    if (fd < 0){
      write(1,"error creating file: ",21);
      write(1, argv[i], strlen(argv[i]));
      write(1, "\n", 1);
    }else{
      close(fd);
    }
  }

  exit(0);
}
