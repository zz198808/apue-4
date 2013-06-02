#include <apue.h>

int main(int argc, char* argv[]) {
  char name[L_tmpnam], line[MAXLINE];
  FILE* fp;

  /* first temp name */
  printf("%s\n", tmpnam(NULL));

  /* second temp name */
  tmpnam(name);
  printf("%s\n", name);

  /* create temp file */
  if ((fp = tmpfile()) == NULL)
    err_sys("tmpfile error");

  /* writes to tmpfile */
  fputs("one line of output\n", fp);
  rewind(fp);
  
  /* then read it back */
  if (fgets(line, sizeof(line), fp) == NULL)
    err_sys("fgets error");
  fputs(line, stdout);

  return 0;
}
