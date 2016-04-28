#ifndef MALLOC_H
#define MALLOC_H

#if __BCC__
#include "types.h"

#define MALLOC_BUF_SIZE 4000

typedef struct MemRecord {
  size_t len;
  struct MemRecord* next;
} MemRecord;


void* malloc(size_t size);
void* realloc(void* old, size_t size);
void free(void* ptr);

void initMemRecord();
void initMemRecordRoot();
bool MemRecord_isSpaceAvailableAfter();
MemRecord* MemRecord_allocateAfter();
MemRecord* MemRecord_findPreviousRecordFor();
MemRecord* MemRecord_deallocateAfter();

#else
  #include "stdlib.h"
#endif

#endif
