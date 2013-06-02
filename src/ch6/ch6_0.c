#include <apue.h>
#include <pwd.h>

int main(int argc, char* argv[]) {
  struct passwd* pwd;

  pwd = getpwnam("muradin");

  printf("User Name:\t%s\n", pwd->pw_name);
  printf("Password:\t%s\n", pwd->pw_passwd);
  printf("User ID:\t%d\n", (int) pwd->pw_uid);
  printf("Group ID:\t%d\n", (int) pwd->pw_uid);
  printf("Real Name:\t%s\n", pwd->pw_gecos);
  printf("Home Dir:\t%s\n", pwd->pw_dir);
  printf("Shell:\t%s\n", pwd->pw_shell);
  
  return 0;
}
