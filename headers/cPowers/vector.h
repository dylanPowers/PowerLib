#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdlib.h>

#include "systemError.h"

/**
 * Vector is a growable array implementation for C. The idea is for it to work
 * similar to a Vector in C++. The implementation is generic so it can
 * hold any data type as long as the type size is given.
 */

// Look, a mutherfuckin vector. Let's reinvent the rock
typedef struct Vector  {
  void* arr; // Array yo
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

inline Vector* initByteVector(Vector* v, size_t initSize,
                              const char* contents, size_t num, SystemErr* e) {
  return initVectorAdvanced(v, sizeof(char), initSize, contents, num, NULL, NULL, e);
}

inline Vector* initDoubleVector(Vector* v, const char* contents, size_t num,
                                SystemErr* e) {
  return initVectorAdvanced(v, sizeof(double), 0, contents, num, NULL, NULL, e);
}

void* Vector_add(Vector*, const void*, SystemErr*);
void* Vector_at(const Vector*, size_t, VectorErr* e);
Vector* Vector_cat(Vector*, const Vector*, VectorErr*, SystemErr*);
Vector* Vector_catPrimitive(Vector*, const void*, size_t, SystemErr*);
Vector* Vector_clear(Vector*);
void Vector_reverse(const Vector*, Vector*, SystemErr*);
void* Vector_last(Vector*, VectorErr*);
void Vector_removeLast(Vector*);

#endif
