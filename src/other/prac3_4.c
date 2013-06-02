#include <apue.h>
#include <fcntl.h>

char buf1[] = "fd1";
char buf2[] = "fd2";
char buf3[] = "fd3";

int main(int argc, char* argv[]) {
  int fd;
  if ((fd = creat("prac3_4.log", FILE_MODE)) == -1) {
    err_sys("creat error.");
  }

  printf("fd = %d\n", fd);
  
  dup2(fd, 0);
  dup2(fd, 1);
  dup2(fd, 2);
  if (fd > 2) {
    printf("\nstdout was redirected.");
    fprintf(stdout, "fd = %d was closed.", fd);
    close(fd);
  } else {
    fprintf(stdout, "fd now equal to %d.", fd);
  }
    

  write(0, buf1, 3);
  write(1, buf2, 3);
  write(2, buf3, 3);
  
  return 0;
}
