#ifndef STRING_VECTOR_H
#define STRING_VECTOR_H

#ifndef __BCC__
#include "stdio.h"
#endif

#include "systemError.h"
#include "vector.h"

typedef Vector String;

String* initString(String*, const char*, SystemErrNoMems*);
String* initStringCp(String*, const String*, SystemErrNoMems*);
void deinitString(String*);

void String_catnprintf(String* str, size_t n, SystemErrNoMems* se, const char* fmt, ...);
char String_charAt(const String*, size_t, VectorErrRange*);
int String_cmp(const String*, const String*);
void String_fgets(String*, FILE*, SystemErrNoMems*);
void String_gets();
void String_nprintf(String* str, size_t n, SystemErrNoMems* se, const char* fmt, ...);
int String_toi(const String* str, int base);
void String_tok(const String* str, Vector* tokenContainer,
                const char* delimiters, SystemErrNoMems* se);

#endif
