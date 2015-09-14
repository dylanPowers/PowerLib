#ifndef STRING_VECTOR_H
#define STRING_VECTOR_H

#include "stdio.h"
#include "vector.h"

typedef Vector String;

String* initString(String*, const char*, VectorErr*);
String* initStringCp(String*, const String*, VectorErr*);
void deinitString(String*);

char String_charAt(String*, size_t, VectorErr*);
int String_cmp(const String*, const String*);
void String_fgets(String*, FILE*, VectorErr*);
void String_tok(String*, Vector*, char*, VectorErr*);

#endif
