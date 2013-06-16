#include "apue.h"

static char buf[16];
int main(int argc, char* argv[])
{
  int int1, int2;
  char line[MAXLINE];

  if (setvbuf(stdout, buf, _IOFBF, 16) != 0) {
    err_sys("setvbuf error");
  }
  
  while (fgets(line, MAXLINE, stdin) != NULL) {
    if (sscanf(line, "%d%d", &int1, &int2) == 2) {
      if (printf("%d\n", int1 + int2) == EOF) {
        err_sys("printf error");
      }
    }
    else {
      if (printf("invalid args\n") == EOF) {
        err_sys("pirntf error");
      }
    }
  }

  return 0;
}
