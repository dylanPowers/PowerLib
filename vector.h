#ifndef VECTOR_H
#define VECTOR_H

#include "stdlib.h"
#include "string.h" // memcpy() has to do with strings apparently

/**
 * Vector is a growable array implementation for C. The idea is for it to work
 * similar to a Vector in C++. The implementation is generic so it can
 * hold any data type as long as the type size is given.
 */

#define _VECTOR_DEFAULT_INIT_SIZE 16

// Look, a mutherfuckin vector. Let's reinvent the rock
typedef struct Vector {
  void* arr; // Array yo
  int length; // Woah! We're told a length?

  // Privates. No touchy!
  int _arrSize; // A size
  void (*_destruct)(void*);
  int _typeSize;
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

Vector* newVector(int typeSize, const void* contents, int num, 
                  void (*deconstructor)(void*));
Vector* newVectorWithSize(int typeSize, int initSize, 
                          const void* contents, int num, 
                          void (*deconstructor)(void*));
Vector* initVector(Vector*, int typeSize, int initSize, 
                   const void* contents, int num, void (*deconstructor)(void*));
void destroyVector(Vector**);
void deinitVector(Vector*);

// The following are aliases for common array types
inline Vector* newCharVector(int initSize, const char* contents, int num) { 
  return newVectorWithSize(sizeof(char), initSize, contents, num, NULL); 
}

inline Vector* initCharVector(Vector* v, int initSize, 
                              const char* contents, int num) {
  return initVector(v, sizeof(char), initSize, contents, num, NULL);
}

inline Vector* newDoubleVector(const double* contents, int num) {
  return newVector(sizeof(double), contents, num, NULL);
}

inline Vector* initDoubleVector(Vector* v, const char* contents, int num) {
  return initVector(v, sizeof(double), 0, contents, num, NULL);
}

inline Vector* newIntVector(const int* contents, int num) {
  return newVector(sizeof(int), contents, num, NULL);
}

inline Vector* newPointerVector(const void* contents, int num, 
                                void (*deconstructor)(void*)) {
  return newVector(sizeof(void*), contents, num, deconstructor);
}

inline Vector* initPointerVector(Vector* v, const void *contents, int num, 
                                 void (*deconstructor)(void *)) {
  return initVector(v, sizeof(void*), 0, contents, num, deconstructor);
}

void* VectorAdd(Vector*, const void* element, VectorErr*);
Vector* VectorCat(Vector*, const Vector*, VectorErr*);
Vector* VectorCatPrimitive(Vector*, const void* arr, int num, VectorErr*);
Vector* VectorClear(Vector*);
void* VectorPtrAt(const Vector*, int index, VectorErr*);

void _VectorAppendNull(const Vector* v);

inline void* _VectorCalcPtrAt(const Vector* v, int index) {
  return ((char*) v->arr) + index * v->_typeSize;
}

inline void* _VectorCalcDanglingPtr(const Vector* v) {
  return _VectorCalcPtrAt(v, v->length);
}


#endif