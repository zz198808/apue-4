/**
 * @file   prac5_1.c
 * @author Lei Mou <lei.mou.uu@gmail.com>
 * @date   Thu Aug  9 21:14:19 2012
 * 
 * @brief  
 * 
 * 
 */
#include <apue.h>

void mysetbuf(FILE *stream, char *buf) {
  if (stream == NULL) {
    setvbuf(stream, buf, _IONBF, BUFSIZ);
  } else {
    /// Question:
    /// how to tell if the stream should use line buffer or full buffer?
    setvbuf(stream, buf, _IOFBF, BUFSIZ);
  }
}

int main(int argc, char* argv[]) {

  return 0;
}
