#include "apue.h"

static char stdin_buf[32];
static char stdout_buf[32];

int main(int argc, char* argv[]) {
  if (setvbuf(stdin, stdin_buf, _IOFBF, 32) != 0) {
    err_sys("setvbuf error!");
  }
  if (setvbuf(stdout, stdout_buf, _IOFBF, 32) != 0) {
    err_sys("setvbuf error!");
  }
  char line[MAXLINE];
  while (fgets(line, MAXLINE, stdin) != NULL) {
    fprintf(stdout, "%s\n", line);
  }

  return 0;
}
