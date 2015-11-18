#ifndef STRING_VECTOR_H
#define STRING_VECTOR_H

#include "stdio.h"
#include "systemError.h"
#include "vector.h"

typedef Vector String;

String* initString(String*, const char*, SystemErr*);
String* initStringCp(String*, const String*, SystemErr*);
void deinitString(String*);

void String_catnprintf(String* str, size_t n, SystemErr* se, const char* fmt, ...);
char String_charAt(const String*, size_t, VectorErr*);
int String_cmp(const String*, const String*);
void String_fgets(String*, FILE*, SystemErr*);
void String_nprintf(String* str, size_t n, SystemErr* se, const char* fmt, ...);
void String_tok(const String* str, Vector* tokenContainer,
                const char* delimiters, SystemErr* se);

#endif
