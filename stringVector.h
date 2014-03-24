#ifndef STRING_VECTOR_H
#define STRING_VECTOR_H

#include "stdio.h"
#include "vector.h"

typedef Vector String;

#define _STRING_VECTOR_INIT_SIZE 64

inline String* newString(const char* contents) { 
  int len = contents ? strlen(contents) : 0;
  return newCharVector(_STRING_VECTOR_INIT_SIZE, contents, len); 
}

inline String* initString(String* str, const char* contents) {
  int len = contents ? strlen(contents) : 0;
  return initCharVector(str, _STRING_VECTOR_INIT_SIZE, contents, len);
}

void StringFgets(String* str, FILE* fd, VectorErr*);

#endif