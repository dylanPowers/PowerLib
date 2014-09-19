#ifndef STRING_VECTOR_H
#define STRING_VECTOR_H

#include "stdio.h"
#include "vector.h"

typedef Vector String;
typedef VectorErr StringErr;

String* newString(const char*);
String* initString(String*, const char*);
String* initStringCp(String*, const String*);

char String_charAt(const String*, int, StringErr*);
void String_fgets(String*, FILE*, StringErr*);
void String_tok(String*, Vector*, String*, StringErr*);

#endif