#ifndef SYSTEM_ERRORS_H
#define SYSTEM_ERRORS_H

#include "errno.h"

typedef enum SystemErr {
  S_E_CLEAR,
  S_E_NOMEMS
} SystemErr;

inline void fatal(SystemErr se) {
  errno = se;
  raise(SIGABRT);
}

#endif
