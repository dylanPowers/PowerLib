#include "systemError.h"

void raiseError(SystemErr se) {
  errno = se;
  if (se == S_E_NOMEMS) {
    fprintf(stderr, "Fatal Error: No more memory available");
  }

  raise(SIGABRT);
}
