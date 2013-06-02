#include <apue.h>
#include <dlfcn.h>

#define MAX_LIB_NUM 256

static void* handles[MAX_LIB_NUM];
static int index = 0;

void load_library(const char* library) {
  if (library != NULL) {
    void* handle = dlopen(library, RTLD_NOW | RTLD_GLOBAL);

    if (handle == NULL) {
      printf("[Error] %s", dlerror());
      return;
    }
    
    handles[index++] = handle;
  }
}

void unload_libraries() {
  for (int i=0; i<index; i++) {
    dlclose(handles[i]);
  }
}
