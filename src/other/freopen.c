#include <stdio.h>

int main(int argc, char* argv[]) {
  FILE* f = freopen("test-freopen.log", "w", stdout);
  printf("This message should be written to test-freopen.log\n");

  fclose(f);
  
  return 0;
}
