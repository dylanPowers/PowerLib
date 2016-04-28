#ifndef VECTOR_H
#define VECTOR_H

#include "types.h"

#include "systemError.h"

#include "malloc.h"

#define _VECTOR_DEFAULT_INIT_SIZE 16


/**
 * Vector is a growable array implementation for C. The idea is for it to work
 * similar to a Vector in C++. The implementation is generic so it can
 * hold any data type as long as the type size is given.
 */

// Look, a mutherfuckin vector. Let's reinvent the rock
typedef struct Vector  {
  void* arr;
  size_t length; // Woah! We're told a length?

  // Privates. No touchy!
  size_t _arrSize; // Allocated array size
  void* (*_copyInitializer)(void*, const void*, Err*);
  void (*_deInitializer)(void*);
  size_t _typeSize;
} Vector;

/**
* These are the various errors that a vector can give.
*/
typedef Err VectorErrIncompatibleTypes; 
typedef Err VectorErrRange;
typedef Err VectorErrEmpty;


Vector* initVector(Vector*, size_t, void* (*)(void*, const void*, Err*),
                   void (*)(void*), SystemErrNoMems*);
Vector* initVectorCp(Vector*, const Vector*, SystemErrNoMems*);
Vector* initVectorAdvanced(Vector*, size_t, size_t, const void*, size_t,
                           void* (*)(void*, const void*, Err*), void (*)(void*),
                           SystemErrNoMems*);
void deinitVector(Vector*);


Vector* initByteVector(Vector* v, size_t initSize, const char* contents,
                              size_t num, SystemErrNoMems*);
Vector* initIntVector(Vector* v, const char* contents, size_t num, SystemErrNoMems* se);
Vector* initDoubleVector(Vector* v, const char* contents, size_t num,
                                SystemErrNoMems*);


void* Vector_add(Vector*, const void*, SystemErrNoMems*);
void* Vector_addEmpty(Vector* v, SystemErrNoMems* se);
void* Vector_at(const Vector*, size_t, VectorErrRange* e);
Vector* Vector_cat(Vector*, const Vector*, VectorErrIncompatibleTypes*, SystemErrNoMems*);
Vector* Vector_catPrimitive(Vector*, const void*, size_t, SystemErrNoMems*);
Vector* Vector_clear(Vector*);
void Vector_reverse(const Vector*, Vector*, SystemErrNoMems*);
void* Vector_last(Vector*, VectorErrEmpty*);
void Vector_removeLast(Vector*);

void* _Vector_calcDanglingPtr(const Vector*);
void _Vector_resize(Vector*, size_t, SystemErrNoMems*);
void* _Vector_appendCopy(Vector*, const void*, Err*);
void _Vector_appendNull(const Vector*);
void* _Vector_calcPtrAt(const Vector*, size_t);
void _Vector_reinit(Vector*, size_t, size_t*, Err*);

#endif
