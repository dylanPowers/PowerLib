#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdlib.h>

/**
 * Vector is a growable array implementation for C. The idea is for it to work
 * similar to a Vector in C++. The implementation is generic so it can
 * hold any data type as long as the type size is given.
 */

/**
* These are the various errors that a vector can give.
*/
typedef enum VectorErr {
  V_E_CLEAR,
  V_E_NOMEMS,
  V_E_INCOMPATIBLE_TYPES,
  V_E_RANGE,
  V_E_EMPTY
} VectorErr;

// Look, a mutherfuckin vector. Let's reinvent the rock
typedef struct Vector  {
  void* arr; // Array yo
  VectorErr e;
  size_t length; // Woah! We're told a length?

  // Privates. No touchy!
  size_t _arrSize; // Allocated array size
  void* (*_copyInitializer)(void*, const void*);
  void (*_deInitializer)(void*);
  size_t _typeSize;
} Vector;

Vector* initVector(Vector*, size_t, void* (*)(void*, const void*),
                   void (*)(void*));
Vector* initVectorCp(Vector*, const Vector*);
Vector* initVectorAdvanced(Vector*, size_t, size_t, const void*, size_t,
                           void* (*)(void*, const void*), void (*)(void*));
void deinitVector(Vector*);

inline Vector* initByteVector(Vector* v, size_t initSize,
                              const char* contents, size_t num) {
  return initVectorAdvanced(v, sizeof(char), initSize, contents, num, NULL, NULL);
}

inline Vector* initDoubleVector(Vector* v, const char* contents, size_t num) {
  return initVectorAdvanced(v, sizeof(double), 0, contents, num, NULL, NULL);
}

void* Vector_add(Vector*, const void*);
Vector* Vector_cat(Vector*, const Vector*);
Vector* Vector_catPrimitive(Vector*, const void*, size_t);
Vector* Vector_clear(Vector*);
void* Vector_at(Vector*, size_t);
void Vector_reverse(const Vector*, Vector*);
void* Vector_last(Vector*);
void Vector_removeLast(Vector*);

#endif