#include <apue.h>
#include <fcntl.h>

char buf1[] = "fd1";
char buf2[] = "fd2";
char buf3[] = "fd3";

int main(int argc, char* argv[]) {
  int fd1, fd2, fd3;

  fd1 = open(argv[1], O_RDWR | O_CREAT | O_TRUNC);
  fd2 = dup(fd1);
  fd3 = open(argv[1], O_RDWR | O_CREAT | O_TRUNC);

  printf("fd1 = %d, fd2 = %d, fd3 = %d\n", fd1, fd2, fd3);

  write(fd1, "fd1", 3);  // fd1
  write(fd2, "fd2", 3);  // fd1fd2
  
  write(fd3, "fd3", 3);  // fd3fd2
  
  return 0;
}
