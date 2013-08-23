#include "apue.h"
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

int
ptym_open(char *pts_name, int pts_namesz)
{
  char *ptr;
  int  fdm;

  strncpy(pts_name, "/dev/ptmx", pts_namesz);
  pts_name[pts_namesz - 1] = '\0';

  fdm = posix_openpt(O_RDWR);
  if (fdm < 0)
    return -1;
  if (grantpt(fdm) < 0) {
    close(fdm);
    return -2;
  }
  if (unlockpt(fdm) < 0) {
    close(fdm);
    return -3;
  }
  if ((ptr = ptsname(fdm)) == NULL) {
    close(fdm);
    return -4;
  }
  
  strncpy(pts_name, ptr, pts_namesz);
  pts_name[pts_namesz - 1] = '\0';
  return fdm;
}

int
ptys_open(char *pts_name)
{
  int fds;
  if ((fds = open(pts_name, O_RDWR)) < 0) {
    return -5;
  }
  return fds;
}

pid_t
pty_fork(int *ptrfdm, char *slave_name, int slave_namesz,
         const struct termios *slave_termios,
         const struct winsize *slave_winsize)
{
  int     fdm, fds;
  pid_t   pid;
  char    pts_name[20];

  if ((fdm = ptym_open(pts_name, sizeof(pts_name))) < 0) {
    err_sys("cannot open master pty: %s, error %d", pts_name, fdm);
  }
  if (slave_name != NULL) {
    strncpy(slave_name, pts_name, slave_namesz);
    slave_name[slave_namesz - 1] = '\0';
  }

  if ((pid = fork()) < 0) {
    return -1;
  }
  else if (pid == 0) {
    if (setsid() < 0) {
      err_sys("setsid error");
    }

    if ((fds = ptys_open(pts_name)) < 0)
      err_sys("can't open slave pty");
    close(fdm);

#if defined(TIOCSCTTY)
    if (ioctl(fds, TIOCSCTTY, (char *) 0) < 0) {
      err_sys("TIOCSCTTY error");
    }
#endif

    if (slave_termios != NULL) {
      if (tcsetattr(fds, TCSANOW, slave_termios) < 0)
        err_sys("tcsetattr error on slave pty");
    }
    if (slave_winsize != NULL) {
      if (ioctl(fds, TIOCSWINSZ, slave_winsize) < 0)
        err_sys("TIOCSWINSZ error on slave pty");
    }

    if (dup2(fds, STDIN_FILENO) != STDIN_FILENO)
      err_sys("dup2 error to stdin");
    if (dup2(fds, STDOUT_FILENO) != STDOUT_FILENO)
      err_sys("dup2 error to stdin");
    if (dup2(fds, STDERR_FILENO) != STDERR_FILENO)
      err_sys("dup2 error to stdin");
    if (fds != STDIN_FILENO && fds != STDOUT_FILENO && fds != STDERR_FILENO)
      close(fds);
    return 0;
  }
  else {
    *ptrfdm = fdm;
    return pid;
  }
}
