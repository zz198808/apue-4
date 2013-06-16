#include "apue.h"
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <unistd.h>

void daemonize(const char* cmd)
{
  int i, fd0, fd1, fd2;
  pid_t pid;
  struct rlimit r1;
  struct sigaction sa;

  umask(0);
  
  if (getrlimit(RLIMIT_NOFILE, &r1) < 0) {
    err_quit("%s: can't get file limit", cmd);
  }

  if ((pid = fork()) < 0) {
    err_quit("%s: can't fork", cmd);
  }
  else if (pid != 0) { /* parent */
    printf("grandparent: pid = %d, ppid = %d, sid = %d\n",
	   getpid(), getppid(), getsid(getpid()));
    exit(0);
  }
  setsid();

  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGHUP, &sa, NULL) < 0) {
    err_quit("%s: cannnot ignore SIGHUP");
  }
  if ((pid = fork()) < 0) {
    err_quit("%s: cannot fork", cmd);
  }
  else if (pid != 0) { /* parent */
    printf("parent: pid = %d, ppid = %d, sid = %d\n",
	   getpid(), getppid(), getsid(getpid()));
    exit(0);
  }
  
  if (chdir("/") < 0) {
    err_quit("%s: cannot change directory to /");
  }

  if (r1.rlim_max == RLIM_INFINITY) {
    r1.rlim_max = 1024;
  }
  for (int i=0; i<r1.rlim_max; i++) {
    close(i);
  }

  fd0 = open("/dev/null", O_RDWR);
  fd1 = dup(0);
  fd2 = dup(0);

  openlog(cmd, LOG_CONS, LOG_DAEMON);
  if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
    syslog(LOG_ERR, "Unexpected file descriptors %d %d %d.\n",
	   fd0, fd1, fd2);
    exit(1);
  }
}

int main(int argc, char* argv[]) 
{
  daemonize(argv[0]);
  
  while (1) {
    sleep(1000);
  }

  return 0;
}
