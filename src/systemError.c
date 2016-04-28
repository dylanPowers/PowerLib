#include "systemError.h"


void raiseError(Err* e) {
#if __BCC__
  printf("%s\n", e->msg);
  exit(1);
#else
  fprintf(stderr, "%s\n", e->msg);
  raise(SIGABRT);
#endif
}
