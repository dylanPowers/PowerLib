#ifndef VECTOR_H
#define VECTOR_H

#include "stdlib.h"

/**
 * Vector is a growable array implementation for C. The idea is for it to work
 * similar to a Vector in C++. The implementation is generic so it can
 * hold any data type as long as the type size is given.
 */

typedef struct Vector Vector;
typedef enum VectorErr VectorErr;

Vector* newVector(int, const void*, int, void* (*)(void*, const void*), 
                  void (*)(void*));
Vector* newVectorWithSize(int, int, const void*, int, 
                          void* (*)(void*, const void*), void (*)(void*));
Vector* initVector(Vector*, int, int, const void*, int, 
                   void* (*)(void*, const void*), void (*)(void*));
void destroyVector(Vector**);
void deinitVector(Vector*);

// The following are aliases for common array types
inline Vector* newByteVector(int initSize, const char* contents, int num) { 
  return newVectorWithSize(sizeof(char), initSize, contents, num, NULL, NULL); 
}

inline Vector* initByteVector(Vector* v, int initSize, 
                              const char* contents, int num) {
  return initVector(v, sizeof(char), initSize, contents, num, NULL, NULL);
}

inline Vector* newDoubleVector(const double* contents, int num) {
  return newVector(sizeof(double), contents, num, NULL, NULL);
}

inline Vector* initDoubleVector(Vector* v, const char* contents, int num) {
  return initVector(v, sizeof(double), 0, contents, num, NULL, NULL);
}

inline Vector* newIntVector(const int* contents, int num) {
  return newVector(sizeof(int), contents, num, NULL, NULL);
}

void* Vector_add(Vector*, const void*, VectorErr*);
Vector* Vector_cat(Vector*, const Vector*, VectorErr*);
Vector* Vector_catPrimitive(Vector*, const void*, int, VectorErr*);
Vector* Vector_clear(Vector*);
void Vector_forEach(const Vector*, void (*)(void*, int));
void* Vector_ptrAt(const Vector*, int, VectorErr*);

#endif