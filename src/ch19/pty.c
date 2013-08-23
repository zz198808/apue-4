#include "apue.h"
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <errno.h>

#ifndef LINUX
#define OPTSTR "+d:einv"
#else
#define OPTSTR "d:einv"
#endif

#define BUFFSIZE 512

static void sig_term(int);
static struct termios	     save_termios;
static volatile sig_atomic_t sigcaught;
static int                   ttysavefd = -1;

static void set_noecho(int);
void        do_driver(char *);
void        loop(int, int);

static enum { RESET, RAW, CBREAK }	ttystate = RESET;

int
tty_reset(int fd)		/* restore terminal's mode */
{
  if (ttystate == RESET)
    return(0);
  if (tcsetattr(fd, TCSAFLUSH, &save_termios) < 0)
    return(-1);
  ttystate = RESET;
  return(0);
}

void
tty_atexit(void)		/* can be set up by atexit(tty_atexit) */
{
  if (ttysavefd >= 0)
    tty_reset(ttysavefd);
}

int
tty_raw(int fd)		/* put terminal into a raw mode */
{
  int				err;
  struct termios	buf;

  if (ttystate != RESET) {
    errno = EINVAL;
    return(-1);
  }
  if (tcgetattr(fd, &buf) < 0)
    return(-1);
  save_termios = buf;	/* structure copy */

  /*
   * Echo off, canonical mode off, extended input
   * processing off, signal chars off.
   */
  buf.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  /*
   * No SIGINT on BREAK, CR-to-NL off, input parity
   * check off, don't strip 8th bit on input, output
   * flow control off.
   */
  buf.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

  /*
   * Clear size bits, parity checking off.
   */
  buf.c_cflag &= ~(CSIZE | PARENB);

  /*
   * Set 8 bits/char.
   */
  buf.c_cflag |= CS8;

  /*
   * Output processing off.
   */
  buf.c_oflag &= ~(OPOST);

  /*
   * Case B: 1 byte at a time, no timer.
   */
  buf.c_cc[VMIN] = 1;
  buf.c_cc[VTIME] = 0;
  if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
    return(-1);

  /*
   * Verify that the changes stuck.  tcsetattr can return 0 on
   * partial success.
   */
  if (tcgetattr(fd, &buf) < 0) {
    err = errno;
    tcsetattr(fd, TCSAFLUSH, &save_termios);
    errno = err;
    return(-1);
  }
  if ((buf.c_lflag & (ECHO | ICANON | IEXTEN | ISIG)) ||
      (buf.c_iflag & (BRKINT | ICRNL | INPCK | ISTRIP | IXON)) ||
      (buf.c_cflag & (CSIZE | PARENB | CS8)) != CS8 ||
      (buf.c_oflag & OPOST) || buf.c_cc[VMIN] != 1 ||
      buf.c_cc[VTIME] != 0) {
    /*
     * Only some of the changes were made.  Restore the
     * original settings.
     */
    tcsetattr(fd, TCSAFLUSH, &save_termios);
    errno = EINVAL;
    return(-1);
  }

  ttystate = RAW;
  ttysavefd = fd;
  return(0);
 }

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

int main(int argc, char *argv[])
{
  int             fdm, c, ignoreeof, interactive, noecho, verbose;
  pid_t           pid;
  char            *driver;
  char            slave_name[20];
  struct termios  orig_termios;
  struct winsize  size;

  interactive = isatty(STDIN_FILENO);
  ignoreeof = 0;
  noecho = 0;
  verbose = 0;
  driver = NULL;

  opterr = 0;
  while ((c = getopt(argc, argv, OPTSTR)) != EOF) {
    switch (c) {
    case 'd':
      driver = optarg;
      break;
    case 'e':
      noecho = 1;
      break;
    case 'i':
      ignoreeof = 1;
      break;
    case 'n':
      interactive = 0;
      break;
    case 'v':
      verbose = 1;
      break;
    case '?':
      err_quit("unrecognized option: -%c", optopt);
    }
  }

  if (optind >= argc) {
    err_quit("usage: pty [ -d driver -einv ] program [ args ... ]");
  }
  
  if (interactive) {
    if (tcgetattr(STDIN_FILENO, &orig_termios) < 0)
      err_sys("tcgetattr error on stdin");
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&size) < 0)
      err_sys("TIOCGWINSZ error");
    pid = pty_fork(&fdm, slave_name, sizeof(slave_name), &orig_termios, &size);
  }
  else {
    pid = pty_fork(&fdm, slave_name, sizeof(slave_name), NULL, NULL);
  }
  
  if (pid < 0) {
    err_sys("fork error");
  }
  else if (pid == 0) {
    if (noecho) {
      set_noecho(STDIN_FILENO);
    }
    if (execvp(argv[optind], &argv[optind]) < 0)
      err_sys("can't execute: %s", argv[optind]);
  }

  if (verbose) {
    fprintf(stderr, "slave name = %s\n", slave_name);
    if (driver != NULL) {
      fprintf(stderr, "driver = %s\n", driver);
    }
  }

  if (interactive && driver == NULL) {
    if (tty_raw(STDIN_FILENO) < 0)	/* user's tty to raw mode */
      err_sys("tty_raw error");
    if (atexit(tty_atexit) < 0)		/* reset user's tty on exit */
      err_sys("atexit error");
  }

  if (driver)
    do_driver(driver);	/* changes our stdin/stdout */

  loop(fdm, ignoreeof);	/* copies stdin -> ptym, ptym -> stdout */

  exit(0);
}

static void
set_noecho(int fd)
{
  struct termios stermios;
  if (tcgetattr(fd, &stermios) < 0)
    err_sys("tcgetattr error");

  stermios.c_lflag &= ~(ECHO|ECHOE|ECHOK|ECHONL);
  stermios.c_oflag &= ~(ONLCR);

  if (tcsetattr(fd, TCSANOW, &stermios) < 0) {
    err_sys("tcsetattr error");
  }
}

Sigfunc*
signal_intr(int signo, Sigfunc *func)
{
  struct sigaction act, oact;
  act.sa_handler = func;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
#ifdef SA_INTERRUPT
  act.sa_flags |= SA_INTERRUPT;
#endif
  if (sigaction(signo, &act, &oact) < 0)
    return SIG_ERR;
  return oact.sa_handler;
}

void
loop(int ptym, int ignoreeof)
{
  pid_t child;
  int   nread;
  char  buf[BUFFSIZE];

  if ((child = fork()) < 0) {
    err_sys("fork error");
  }
  else if (child == 0) {
    while (1) {
      if ((nread = read(STDIN_FILENO, buf, BUFFSIZE)) < 0) {
        err_sys("read error from stdin");
      }
      else if (nread == 0)
        break;
      if (write(ptym, buf, nread) != nread) {
        err_sys("write error to master pty");
      }
    }
    if (ignoreeof == 0) {
      kill(getppid(), SIGTERM);
    }
    exit(0);
  }
  if (signal_intr(SIGTERM, sig_term) == SIG_ERR) {
    err_sys("signal_intr error for SIGTERM");
  }
  while (1) {
    if ((nread = read(ptym, buf, BUFFSIZE)) <= 0)
      break;
    if (write(STDOUT_FILENO, buf, nread) != nread)
      err_sys("writen error to stdout");
  }

  if (sigcaught == 0)
    kill(child, SIGTERM);
}

static void
sig_term(int signo)
{
  sigcaught = 1;
}

int
s_pipe(int fd[2])
{
  return(socketpair(AF_UNIX, SOCK_STREAM, 0, fd));
}

void
do_driver(char *driver)
{
  pid_t	child;
  int		pipe[2];

  /*
   * Create a stream pipe to communicate with the driver.
   */
  if (s_pipe(pipe) < 0)
    err_sys("can't create stream pipe");

  if ((child = fork()) < 0) {
    err_sys("fork error");
  } else if (child == 0) {		/* child */
    close(pipe[1]);

    /* stdin for driver */
    if (dup2(pipe[0], STDIN_FILENO) != STDIN_FILENO)
      err_sys("dup2 error to stdin");

    /* stdout for driver */
    if (dup2(pipe[0], STDOUT_FILENO) != STDOUT_FILENO)
      err_sys("dup2 error to stdout");
    if (pipe[0] != STDIN_FILENO && pipe[0] != STDOUT_FILENO)
      close(pipe[0]);

    /* leave stderr for driver alone */
    execlp(driver, driver, (char *)0);
    err_sys("execlp error for: %s", driver);
  }

  close(pipe[0]);		/* parent */
  if (dup2(pipe[1], STDIN_FILENO) != STDIN_FILENO)
    err_sys("dup2 error to stdin");
  if (dup2(pipe[1], STDOUT_FILENO) != STDOUT_FILENO)
    err_sys("dup2 error to stdout");
  if (pipe[1] != STDIN_FILENO && pipe[1] != STDOUT_FILENO)
    close(pipe[1]);

  /*
   * Parent returns, but with stdin and stdout connected
   * to the driver.
   */
}
