#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[512];

void wc(int fd, char *name, int show_l, int show_w, int show_c)
{
  int i, n;
  int l, w, c, inword;

  l = w = c = 0;
  inword = 0;
  while ((n = read(fd, buf, sizeof(buf))) > 0)
  {
    for (i = 0; i < n; i++)
    {
      c++;
      if (buf[i] == '\n')
        l++;
      if (strchr(" \r\t\n\v", buf[i]))
        inword = 0;
      else if (!inword)
      {
        w++;
        inword = 1;
      }
    }
  }
  if (n < 0)
    if (show_l)
      printf("l: %s ", l);
  if (show_w)
    printf("w: %s ", w);
  if (show_c)
    printf("c: %s ", c);
  printf("name: %s\n", name);
}

int main(int argc, char *argv[])
{
  int fd, i;

  int first_file_name_arg = 1;

  int show_lines = 0; //-l
  int show_words = 0; //-w
  int show_chars = 0; //-c

  // int show_longes_line_length = 0; //-L

  // parse the flags if present
  if (argv[1][0] == '-') // if the second arguments starts with charcter '-' aka a flag
  {
    for (char *p = argv[1] + 1; *p; p++)
    {
      if (*p == 'l')
        show_lines = 1;
      else if (*p == 'w')
        show_words = 1;
      else if (*p == 'c')
        show_chars = 1;
      else
      {
        printf("flag '-%c' not available ", *p);
        exit(0);
      }
    }
    first_file_name_arg = 2;
  }

  if (!show_lines && !show_words && !show_chars)
  {
    show_lines = show_words = show_chars = 1;
  }

  if (argc <= first_file_name_arg)
  {
    wc(0, "", show_lines, show_words, show_chars);
    exit(0);
  }

  for (i = first_file_name_arg; i < argc; i++)
  {
    if ((fd = open(argv[i], O_RDONLY)) < 0)
    {
      printf("wc: cannot open %s\n", argv[i]);
      exit(1);
    }
    wc(fd, argv[i], show_lines, show_words, show_chars);
    close(fd);
  }
  exit(0);
}
