#ifndef STRING_VECTOR_H
#define STRING_VECTOR_H

#include "stdio.h"
#include "vector.h"

typedef Vector String;
typedef VectorErr StringErr;

String* newString(const char*);
String* initString(String*, const char*);
String* initStringCp(String*, const String*);

void String_fgets(String* str, FILE* fd, StringErr*);
char String_charAt(const String*, int, StringErr*);

#endif