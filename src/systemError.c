#include "systemError.h"


void raiseError(Err* e) {
#if __BCC__
  printf("\n!==============! Fatal error !==============!\n");
  printf("%s\n", e->msg);
  printf("Hit <enter> to exit\n");
  getc();
  putc('\n');
  exit(1);
#else
  fprintf(stderr, "%s\n", e->msg);
  raise(SIGABRT);
#endif
}
