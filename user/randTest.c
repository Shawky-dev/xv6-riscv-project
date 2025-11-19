#include<kernel/types.h>
#include<kernel/stat.h>
#include<user/user.h>

//test file for rand syscall
//allow user to pick how many random numbers to generate
int
main(int argc, char *argv[])
{
  //check if user needs help
  if(argc == 2 && strcmp(argv[1], "?") == 0)
  {
    printf("Usage: randTest number_of_randoms\n");
    exit(0);
  }

  //check for correct number of arguments
  if(argc != 2)
  {
    printf("Error: Expected 1 argument, got %d\n", argc - 1);
    exit(1);
  }

  //convert argument to integer
  int num_of_randoms = atoi(argv[1]);
  if(num_of_randoms <= 0)
  {
    printf("Error: number_of_randoms must be a positive integer\n");
    exit(1);
  }
  else
  {
    printf("Generating %d random numbers:\n", num_of_randoms);
    for(int i = 0; i < num_of_randoms; i++)
    {
      int rand_num = myrand();
      printf("Random number %d: %d\n", i + 1, rand_num);
    }
  }

  exit(0);
}
