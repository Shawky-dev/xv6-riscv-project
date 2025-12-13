#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define MAXLINE 512
char buf1[MAXLINE];
char buf2[MAXLINE];

int readline(int fd,  char*buf, int max){
  int i, cc;
  char c;

  for(i = 0; i < max - 1; i++){
    cc = read(fd, &c, 1);
    if(cc < 1) break;
    buf[i] = c;
    if(c == '\n'){
      i++;
      break;
    }
  }

  buf[i] = '\0';
  return i;
}

void diff(int fd1, int fd2){
  int line = 1;
  int n1, n2;
  int identical = 1;

  while(1){
    n1 = readline(fd1, buf1, MAXLINE);
    n2 = readline(fd2, buf2, MAXLINE);

    if(n1 == 0 && n2 == 0) break;

    if(n1 == 0){
      printf("%d > %s\n", line, buf2);
      identical = 0;
      line++;
      continue;
    }

    if(n2 == 0){
      printf("%d > %s\n", line, buf1);
      identical = 0;
      line++;
      continue;
    }

    if(strcmp(buf1,buf2) != 0){
      printf("%d < %s\n", line, buf1);
      printf("%d > %s\n", line, buf2);
      identical = 0;
    }

    line++;
  }

  if(identical) printf("Files are identical\n");

}


int main(int argc, char* argv[]){
  if((argc == 2 && strcmp(argv[1], "?") == 0))
  {
    printf("Usage: diff <file1_path> <file2_path>\n");
    exit(0);
  }

  if (argc != 3)
  {
    printf("Error: Command takes 2 argument\n");
    exit(1);
  }

  char* file1 = argv[1],* file2 = argv[2];
  int fd1,fd2;

  if((fd1 = open(file1, 0)) < 0){
    printf("diff: cannot open %s", file1);
    exit(0);
  }

  if((fd2 = open(file2, 0)) < 0){
    printf("diff: cannot open %s", file2);
    exit(0);
  }

  diff(fd1, fd2);

  close(fd1);
  close(fd2);
  exit(0);
}
