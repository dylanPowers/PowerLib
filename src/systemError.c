#include <stdarg.h>
#include "systemError.h"

void raiseError(SystemErr se) {
  errno = se;
  if (se == S_E_NOMEMS) {
    fprintf(stderr, "Fatal Error: No more memory available");
  }

  raise(SIGABRT);
}

void raiseErrMsg(char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  vfprintf(stderr, fmt, vl);
  va_end(vl);

  raise(SIGABRT);
}
