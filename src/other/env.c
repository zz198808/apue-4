#include <stdio.h>
#include <assert.h>

extern char **environ;

int main(int argc, char* argv[]) {
  int i = 0;
  /* while (environ[i]) { */
  /*   printf("%s\n", environ[i]); */
  /*   ++i; */
  /* } */

  int *p  = (int*) argv;
  printf("argc = %d\n", *(p-1));

  assert(*(p-1) == argc);
  while (i < argc) {
    printf("Argument %d: %s\n", i, (char *) *(p+i));
    i++;
  }

  p+=i;
  p++;

  printf("Environment Pointer: %x\n", (unsigned int) p);
  printf("environ = %x\n", (unsigned int) environ);

  while (*p) {
    printf("%s\n", (char *) *p);
    p++;
  }

  p++;

  
  return 0;
}
