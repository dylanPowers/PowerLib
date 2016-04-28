#ifndef SYSTEM_ERRORS_H
#define SYSTEM_ERRORS_H

#define E_MSG_MAX_LEN 1024

typedef struct Err {
  char any;
  char msg[E_MSG_MAX_LEN];
} Err;

typedef Err SystemErrNoMems;

void raiseError(Err* e);

#endif
