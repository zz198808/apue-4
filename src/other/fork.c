#include <apue.h>

int main(int argc, char* argv[]) {
  for (int i=0; i<2; i++) {
    fork();
    /* printf("ppid = %d, pid = %d, i = %d\n", getppid(), getpid(), i); */
    printf("-");
  }
  return 0;
}
