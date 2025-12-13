#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
  if((argc == 2 && strcmp(argv[1], "?") == 0))
  {
    printf("Usage: mv <filenameDirectory_old> <filenameDirectory_new>\n");
    exit(0);
  }

  if (argc != 3)
  {
    printf("Error: Command takes 2 argument\n");
    exit(1);
  }

    char* old = argv[1];
    char* new = argv[2];

    if(link(old, new) < 0){
      printf("mv: link %s %s failed\n",old, new);
      exit(0);
    }

    if(unlink(old) < 0){
      printf("mv: unlink %s failed\n",old);
      exit(0);
    }

    exit(0);
}
