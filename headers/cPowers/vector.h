#ifndef VECTOR_H
#define VECTOR_H

#include "stdlib.h"

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
  void* (*_copyInitializer)(void*, const void*);
  void (*_deInitializer)(void*);
  size_t _typeSize;
} Vector;

/**
* These are the various errors that a vector can give.
*/
typedef enum VectorErr {
  CLEAR,
  E_NOMEMS,
  E_INCOMPATIBLE_TYPES,
  E_RANGE
} VectorErr;

Vector* newVector(size_t, const void*, size_t, void* (*)(void*, const void*),
                  void (*)(void*));
Vector* newVectorWithSize(size_t, size_t, const void*, size_t,
                          void* (*)(void*, const void*), void (*)(void*));
Vector* initVector(Vector*, size_t, size_t, const void*, size_t,
                   void* (*)(void*, const void*), void (*)(void*));
void destroyVector(Vector**);
void deinitVector(Vector*);

// The following are aliases for common array types
inline Vector* newByteVector(size_t initSize, const char* contents, size_t num) {
  return newVectorWithSize(sizeof(char), initSize, contents, num, NULL, NULL); 
}

inline Vector* initByteVector(Vector* v, size_t initSize,
                              const char* contents, size_t num) {
  return initVector(v, sizeof(char), initSize, contents, num, NULL, NULL);
}

inline Vector* newDoubleVector(const double* contents, size_t num) {
  return newVector(sizeof(double), contents, num, NULL, NULL);
}

inline Vector* initDoubleVector(Vector* v, const char* contents, size_t num) {
  return initVector(v, sizeof(double), 0, contents, num, NULL, NULL);
}

inline Vector* newIntVector(const int* contents, size_t num) {
  return newVector(sizeof(int), contents, num, NULL, NULL);
}

void* Vector_add(Vector*, const void*, VectorErr*);
Vector* Vector_cat(Vector*, const Vector*, VectorErr*);
Vector* Vector_catPrimitive(Vector*, const void*, size_t, VectorErr*);
Vector* Vector_clear(Vector*);
void Vector_forEach(const Vector*, void (*)(void*, int));
void* Vector_ptrAt(const Vector*, int, VectorErr*);

#endif