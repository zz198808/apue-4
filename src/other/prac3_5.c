#include <apue.h>
#include <fcntl.h>
#include <errno.h>

char buf1[] = "fd1";
char buf2[] = "fd2";
char buf3[] = "fd3";

int main(int argc, char* argv[]) {
  /* int fd = 1; */

  /* dup2(fd, 0); */
  /* printf("fd = %d\n", fd); */
  
  /* dup2(fd, 1); */
  /* printf("fd = %d\n", fd); */
  
  /* dup2(fd, 2); */
  /* printf("fd = %d\n", fd);   */

  /* if (fd > 2) { */
  /*   printf("fd = %d\n", fd); */
  /*   close(fd); */
  /* } */

  fprintf(stderr, "%s\n", "message to stderr");
  fprintf(stdout, "%s\n", "message to stdout");
  
  return 0;
}
