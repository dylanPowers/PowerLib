#ifndef STRING_VECTOR_H
#define STRING_VECTOR_H

#include "stdio.h"
#include "string.h"
#include "vector.h"

typedef Vector String;

String* newString(const char* contents);
String* initString(String* str, const char* contents);

void StringFgets(String* str, FILE* fd, VectorErr*);

#endif