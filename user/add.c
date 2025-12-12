#include<kernel/types.h>
#include<kernel/stat.h>
#include<user/user.h>

int
main(int argc, char *argv[])
{
  //notes for self to understand argument cont
  //ex: add(1) number1(2) number2(3)
  // this means that this program has 3 arguments

  //check if user needs help
  //reminder strcmp returns 0 if strings are equal
  if((argc == 2 && strcmp(argv[1], "?") == 0))
  {
    printf("Usage: add <number1> <number2>\n");
    exit(0);
  }

  if (argc != 3)
  {
    printf("Error: Command takes 2 argument\n");
    exit(1);
  }

  //covert arguments to integers
  int num1 = atoi(argv[1]);
  int num2 = atoi(argv[2]);

  int total = num1 + num2;

  printf("the sum of %d and %d is %d\n", num1, num2, total);

  exit(0);
}
