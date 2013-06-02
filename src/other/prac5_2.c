/**
 * @file   prac5_2.c
 * @author Lei Mou <lei.mou.uu@gmail.com>
 * @date   Thu Aug  9 21:16:10 2012
 * 
 * @brief  Test fgets and fputs when buffer size is less than string length.
 * 
 * 
 */
#include <apue.h>

#ifdef MAXLINE
# undef MAXLINE
#endif

#define MAXLINE 4

int main(int argc, char* argv[]) {
  char buffer[MAXLINE];

  while (fgets(buffer, MAXLINE, stdin) != NULL) {
    if (fputs(buffer, stdout) == EOF) {
      err_sys("output error");
    }
  }

  if (ferror(stdin))
    err_sys("input error");

  int ret = printf("%s", "\0");
  printf("%d\n", ret);
  
  return 0;
}
