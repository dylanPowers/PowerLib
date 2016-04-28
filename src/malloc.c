#include "malloc.h"

#include "systemError.h"

char buf[MALLOC_BUF_SIZE];
bool initialized = false;
size_t totalBytesAvailable = MALLOC_BUF_SIZE - sizeof(MemRecord);


void* malloc(size_t size) {
  MemRecord* recordIter;
  MemRecord* new = NULL;
  bool spaceAvailableAfter;
  if (!initialized) {
    initMemRecordRoot();
  }

  recordIter = (MemRecord*) buf;
  spaceAvailableAfter = MemRecord_isSpaceAvailableAfter(recordIter, size);
  while (recordIter->next != NULL && !spaceAvailableAfter) {
    recordIter = recordIter->next;
    spaceAvailableAfter = MemRecord_isSpaceAvailableAfter(recordIter, size);
  }

  if (spaceAvailableAfter) {
    new = MemRecord_allocateAfter(recordIter, size);
  } else {
    return NULL;
  }

  totalBytesAvailable -= new->len + sizeof(MemRecord);
  printf("Allocated %u+ %uB @ %x. %uB available\n", 
    new->len, sizeof(MemRecord), new + 1, totalBytesAvailable);
  return new + 1;
}

void* realloc(void* old, size_t size) {
  void* new;
  printf("Reallocating: ");
  new = malloc(size);
  if (!new) {
    return NULL;
  }

  memcpy(new, old, ((MemRecord*) ((char*) old - sizeof(MemRecord)))->len);
  free(old);
  return new;
}

void free(void* ptr) {
  Err e;
  MemRecord* m = MemRecord_findPreviousRecordFor((MemRecord*) buf, ptr);
  if (!m) {
    e.any = true;
    sprintf(e.msg, "Invalid ptr to free @ %x", ptr);
    raiseError(&e);
  }

  totalBytesAvailable += m->next->len + sizeof(MemRecord);
  printf("Freed %u+ %uB @ %x. %uB available\n",
    m->next->len, sizeof(MemRecord), ptr,
    totalBytesAvailable);
  MemRecord_deallocateAfter(m);
}

void initMemRecord(MemRecord* m, size_t len) {
  m->len = len;
  m->next = NULL;
}

void initMemRecordRoot() {
  initMemRecord(buf, 0);
  initialized = true;
}


bool MemRecord_isSpaceAvailableAfter(MemRecord* record, size_t requestSize) {
  size_t requestedEnd = (char*) (record + 1) + record->len + requestSize + sizeof(MemRecord);
  return (record->next == NULL && requestedEnd < buf + MALLOC_BUF_SIZE) ||
    (record->next != NULL && requestedEnd < record->next);
}

MemRecord* MemRecord_allocateAfter(MemRecord* m, size_t len) {
  MemRecord* new = (char*) (m + 1) + m->len;
  MemRecord* next = m->next;
  initMemRecord(new, len);
  m->next = new;
  new->next = next;
  return new;
}

MemRecord* MemRecord_findPreviousRecordFor(MemRecord* m, void* ptr) {
  while (m->next != NULL && m->next + 1 != ptr) {
    m = m->next;
  }

  if (m->next == NULL) {
    return NULL;
  }

  return m;
}

MemRecord* MemRecord_deallocateAfter(MemRecord* m) {
  m->next = m->next->next;
}
