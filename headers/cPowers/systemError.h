#ifndef SYSTEM_ERRORS_H
#define SYSTEM_ERRORS_H

#include <stdio.h>
#include "errno.h"
#include "signal.h"

typedef enum SystemErr {
  S_E_CLEAR,
  S_E_NOMEMS
} SystemErr;

void raiseError(SystemErr se);
void raiseErrMsg(char* fmt, ...);

#endif
