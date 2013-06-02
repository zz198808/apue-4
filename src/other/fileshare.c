/* fileshare.c
 * 
 * The purpose of this program is to:
 * 1. Test whether the open file descriptors will be inherited from its parent process.
 * 2. Test how different process can operate on the same file.
 *
*/

#include <apue.h>
#include <fcntl.h>

char buf1[] = "abcdefghijklm";
char buf2[] = "nopqrstuvwxyz";
  
int main(int argc, char* argv[]) {
  int fd;

  if ((fd = open("file.share", O_WRONLY | O_CREAT | O_TRUNC, FILE_MODE)) < 0) {
    err_sys("File creation failed.");
  }

  pid_t pid;
  if ((pid = fork()) < 0) {
    err_sys("fork error.");
  }

  // The child process write to the second part of the file.
  if (pid == 0) {
    if (lseek(fd, 13, SEEK_SET) == -1) {
      err_sys("lseek error");
    }

    if (write(fd, buf2, 13) != 13) {
      err_sys("buf2 write error");
    }
  }
  else {
    // The parent process write to the first part of the file.
    if (write(fd, buf1, 13) != 13) {
      err_sys("buf1 write error");
    }
  }

  return 0;
}
