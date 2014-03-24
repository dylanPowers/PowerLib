#include "stringVector.h"

/**
 * Allocates memory for a new string using an underlying Vector implementation.
 * Any Vector function can be used on a String. [contents] is a primitive 
 * c string that String can be initialized to. The length of contents is 
 * determined by running strlen(). initString() is similar except memory isn't
 * allocated for a new String.
 * @param  contents Primitive C string to initialize to.
 * @return          A pointer to the initialized string.
 */
extern String* newString(const char *contents);
extern String* initString(String* str, const char* contents);

/**
 * String vector version of fgets. Works exactly like fgets except it's not
 * limited to a predefined string size. 
 * The Vector given as [str] better be of char* type!
 */
void StringFgets(String* str, FILE* fd, VectorErr* e) {
  if (str->_typeSize != sizeof(char)) {
    *e = E_INCOMPATIBLE_TYPES;
    return;
  }

  // Let's be sure to start off clean to prevent bugs, especially with strlen().
  VectorClear(str);

  fgets(str->arr, str->_arrSize, fd);
  str->length = strlen(str->arr);
  while (*((char*) VectorPtrAt(str, str->length - 1, e)) != '\n' && 
         !feof(fd) && !*e) {
    char tmpStr[1024];
    fgets(tmpStr, 1024, fd);
    int len = strlen(tmpStr);
    VectorCatPrimitive(str, tmpStr, len, e);
  }
}