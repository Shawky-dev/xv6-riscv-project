#include<kernel/types.h>
#include<kernel/stat.h>
#include<user/user.h>
#include<kernel/datetime.h>

//user code for datetime syscall
int
main(int argc, char *argv[])
{
  //check if user needs help
  if(argc == 2 && strcmp(argv[1], "?") == 0)
  {
    printf("Usage: datetime\n");
    exit(0);
  }
  //check for correct number of arguments
  if(argc != 1)
  {
    printf("Error: Expected 0 arguments, got %d\n", argc - 1);
    exit(1);
  }

  struct real_date_time dt;

  if(datetime((uint64*)&dt) < 0)
  {
    printf("Error: datetime syscall failed\n");
    exit(1);
  }
  else
  {
    printf("Current Date and time is\n date: %d-%d-%d\n time: %d:%d:%d\n",dt.year,dt.month,dt.day,dt.hour,dt.minute,dt.second);
  }

  exit(0);
}
