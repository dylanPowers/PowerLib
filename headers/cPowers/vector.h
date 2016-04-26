#ifndef VECTOR_H
#define VECTOR_H

#include "types.h"

#ifndef __BCC__
  #include <stdlib.h>
#endif

#include "systemError.h"

#if __BCC__
  #define _VECTOR_DEFAULT_INIT_SIZE 1000
#else
  #define _VECTOR_DEFAULT_INIT_SIZE 16
#endif

/**
 * Vector is a growable array implementation for C. The idea is for it to work
 * similar to a Vector in C++. The implementation is generic so it can
 * hold any data type as long as the type size is given.
 */

// Look, a mutherfuckin vector. Let's reinvent the rock
typedef struct Vector  {
#if __BCC__
  char arr[1000]; // Array yo
#else
  void* arr;
#endif  
  size_t length; // Woah! We're told a length?

  // Privates. No touchy!
  size_t _arrSize; // Allocated array size
  void* (*_copyInitializer)(void*, const void*, SystemErr*);
  void (*_deInitializer)(void*);
  size_t _typeSize;
} Vector;

/**
* These are the various errors that a vector can give.
*/
typedef enum VectorErr {
  V_E_CLEAR,
  V_E_INCOMPATIBLE_TYPES,
  V_E_RANGE,
  V_E_EMPTY
} VectorErr;

Vector* initVector(Vector*, size_t, void* (*)(void*, const void*, SystemErr*),
                   void (*)(void*), SystemErr*);
Vector* initVectorCp(Vector*, const Vector*, SystemErr*);
Vector* initVectorAdvanced(Vector*, size_t, size_t, const void*, size_t,
                           void* (*)(void*, const void*, SystemErr*), void (*)(void*),
                           SystemErr*);
void deinitVector(Vector*);


Vector* initByteVector(Vector* v, size_t initSize, const char* contents,
                              size_t num, SystemErr*);
Vector* initIntVector(Vector* v, const char* contents, size_t num, SystemErr* se);
Vector* initDoubleVector(Vector* v, const char* contents, size_t num,
                                SystemErr*);


void* Vector_add(Vector*, const void*, SystemErr*);
void* Vector_addEmpty(Vector* v, SystemErr* se);
void* Vector_at(const Vector*, size_t, VectorErr* e);
Vector* Vector_cat(Vector*, const Vector*, VectorErr*, SystemErr*);
Vector* Vector_catPrimitive(Vector*, const void*, size_t, SystemErr*);
Vector* Vector_clear(Vector*);
void Vector_reverse(const Vector*, Vector*, SystemErr*);
void* Vector_last(Vector*, VectorErr*);
void Vector_removeLast(Vector*);

void* _Vector_calcDanglingPtr(const Vector*);
void _Vector_resize(Vector*, size_t, SystemErr*);

#endif
