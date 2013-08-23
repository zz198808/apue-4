#include "apue.h"
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAXSLEEP   128
#define MAXADDRLEN 256
#define BUFLEN     128

int connect_retry(int sockfd, const struct sockaddr* addr, socklen_t alen)
{
  int nsec;

  for (nsec = 1; nsec < MAXSLEEP; nsec <<= 1) {
    if (connect(sockfd, addr, alen) == 0) {
      return 0;
    }
    if (nsec <= MAXSLEEP / 2) {
      sleep(nsec);
    }
  }
  return -1;
}

void
print_uptime(int sockfd)
{
  int n;
  char buf[BUFLEN];

  while ((n = recv(sockfd, buf, BUFLEN, 0)) > 0) {
    write(STDOUT_FILENO, buf, n);
  }

  if (n < 0) {
    perror("recv error");
  }
}

int main(int argc, char *argv[])
{
  struct addrinfo *ailist, *aip;
  struct addrinfo hint;
  int             sockfd, err;

  if (argc != 2) {
    fprintf(stderr, "usage: ruptime hostname\n");
    exit(1);
  }
  hint.ai_flags     = 0;
  hint.ai_family    = 0;
  hint.ai_socktype  = SOCK_STREAM;
  hint.ai_protocol  = 0;
  hint.ai_addrlen   = 0;
  hint.ai_canonname = NULL;
  hint.ai_addr      = NULL;
  hint.ai_next      = NULL;

  if ((err = getaddrinfo(argv[1], "ruptime", &hint, &ailist)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(err));
    exit(1);
  }
  for (aip = ailist; aip != NULL; aip = aip->ai_next) {
    if ((sockfd = socket(aip->ai_family, SOCK_STREAM, 0)) < 0) {
      err = errno;
    }
    if (connect_retry(sockfd, aip->ai_addr, aip->ai_addrlen) < 0) {
      err = errno;
    }
    else {
      print_uptime(sockfd);
      exit(0);
    }
    printf("can't connect to %s: %s\n", argv[1], strerror(err));
  }
  exit(1);
}
