#include<kernel/types.h>
#include<kernel/stat.h>
#include<user/user.h>

//command to calulcate factorial of a number
int
main(int argc, char *argv[])
{
  //check if user needs help
  if(argc == 2 && strcmp(argv[1], "?") == 0)
  {
    printf("Usage: fact number\n");
    exit(0);
  }

  //check for correct number of arguments
  if(argc != 2)
  {
    printf("Error: Expected 1 argument, got %d\n", argc - 1);
    exit(1);
  }

  //convert argument to integer
  int num = atoi(argv[1]);

  //check for negative input
  if(num < 0)
  {
    printf("Error: Factorial is not defined for negative numbers\n");
    exit(1);
  }


  //calculate factorial
  int result = 1;
  if(num == 0)
  {
    result = 1;
  }
  else
  {
    for(int i = 1; i <= num; i++)
  {
    result *= i;
  }
  }

  printf("The factorial of %d is %d\n", num, result);

  exit(0);
}
