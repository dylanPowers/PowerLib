#ifndef STRING_VECTOR_H
#define STRING_VECTOR_H

#include "stdio.h"
#include "vector.h"

typedef Vector String;
typedef VectorErr StringErr;

String* initString(String*, const char*);
String* initStringCp(String*, const String*);
void deinitString(String*);

char String_charAt(String*, size_t);
int String_cmp(const String*, const String*);
void String_fgets(String*, FILE*);
void String_tok(String*, Vector*, char*);

#endif