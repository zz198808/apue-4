#include <apue.h>
#include <sys/wait.h>

void init_path() {
  char buffer[MAXLINE] = "./bin;";
  strcat(buffer, getenv("PATH"));
  setenv("PATH", buffer, 1);
}

int main(int argc, char* argv[]) {
  char buf[MAXLINE];
  pid_t pid;
  int status;

  init_path();

  printf("%% ");  
  while (fgets(buf, MAXLINE, stdin) != NULL) {
    if (buf[strlen(buf) - 1] == '\n') {
      buf[strlen(buf) - 1] = '\0';
    }

    if ((pid = fork()) < 0 ) {
      err_sys("fork error");
    }
    else if (pid == 0) { // child process
      execlp(buf, buf, (char *) 0);
      err_ret("couldn't execute: %s", buf);
      exit(127);
    }

    // parent process
    if ((pid = waitpid(pid, &status, 0)) < 0) {
      err_sys("waitpid error");
    }
    printf("%% ");
  }
  
  return 0;
}
