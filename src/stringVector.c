#include "stringVector.h"

#include "string.h"

#define _STRING_VECTOR_INIT_SIZE 64

/**
 * Allocates memory for a new string using an underlying Vector implementation.
 * Any Vector function can be used on a String. [contents] is a primitive 
 * c string that String can be initialized to. The length of contents is 
 * determined by running strlen(). initString() is similar except memory isn't
 * allocated for a new String.
 * @param  contents Primitive C string to initialize to.
 * @return          A pointer to the initialized string.
 */
//String* newString(const char *contents) {
//  size_t len = contents ? strlen(contents) : 0;
//  return newByteVector(_STRING_VECTOR_INIT_SIZE, contents, len);
//}

String* initString(String* str, const char* contents) {
  size_t len = contents ? strlen(contents) : 0;
  return initByteVector(str, _STRING_VECTOR_INIT_SIZE, contents, len);
}

String* initStringCp(String* str, const String* copyString) {
  return initByteVector(str, copyString->_arrSize, copyString->arr,
                        copyString->length);
}

void deinitString(String* str) {
  deinitVector(str);
}

char String_charAt(const String* str, int index, StringErr* e) {
  return *(char*) Vector_ptrAt(str, index, e);
}

/**
 * String vector version of fgets. Works exactly like fgets except it's not
 * limited to a predefined string size.
 * The Vector given as [str] better be of char* type!
 */
void String_fgets(String* str, FILE* fd, StringErr* e) {
  if (str->_typeSize != sizeof(char)) {
    *e = E_INCOMPATIBLE_TYPES;
    return;
  }

  // Let's be sure to start off clean to prevent bugs, especially with strlen().
  Vector_clear(str);

  fgets(str->arr, str->_arrSize, fd);
  str->length = strlen(str->arr);
  while (String_charAt(str, str->length - 1, e) != '\n' && !feof(fd) && !*e) {
    char tmpStr[1024];
    fgets(tmpStr, 1024, fd);
    size_t len = strlen(tmpStr);
    Vector_catPrimitive(str, tmpStr, len, e);
  }
}

void String_tok(String* str, Vector* tokenContainer, String* delimiters,
                StringErr* e) {
  char* token = strtok(str->arr, delimiters->arr);
  while (token != NULL) {
    String strToken;
    initString(&strToken, token);
    Vector_add(tokenContainer, &strToken, e);
    token = strtok(NULL, delimiters->arr);
  }
}