#include "vector.h"

#include "string.h" // memcpy() has to do with strings apparently

#define _VECTOR_DEFAULT_INIT_SIZE 16

void* _Vector_appendCopy(Vector*, const void*);
void _Vector_appendNull(const Vector*);
void* _Vector_calcPtrAt(const Vector*, int);
void* _Vector_calcDanglingPtr(const Vector*);
void _Vector_resize(Vector *, size_t, VectorErr *);

/**
 * This function allocates memory for a new Vector and appropriately
 * initializes it. 
 * Know that [contents] is an optional argument. Set it to NULL or
 * [num] to 0 and it will be ignored.
 */
Vector* newVector(size_t typeSize, const void* contents, size_t num,
                  void* (*initializer)(void*, const void*),
                  void (*deInitializer)(void*)) {
  return newVectorWithSize(typeSize, 0, contents, num, initializer, 
                           deInitializer);
}

Vector* newVectorWithSize(size_t typeSize, size_t initSize,
                          const void* contents, size_t num,
                          void* (*initializer)(void*, const void*),
                          void (*deInitializer)(void*)) {
  Vector* vec = (Vector*) malloc(sizeof(Vector));
  return initVector(vec, typeSize, initSize, contents, num, initializer, 
                    deInitializer);
}

/**
 * Initializes a Vector's parameters. [v] is the Vector we wish to
 * initialize. [typesize] is the size in bytes as given by sizeof() of the
 * type this vector must hold. The remaining parameters are optional
 * (somewhat). [initSize] is the initial length that the underlying array
 * primitive should be initialized to, and will be ignored if it's < 2.
 * However, if [num] is given, then the
 * greater of [initSize] or [num] is used. [contents] is an optional
 * initialization array that will be copied into the vector. [num] is the
 * number of elements in [contents]. [deconstructor] is the function that is
 * used to dispose of elements in the vector when needed and is required for
 * anything that's been dynamically allocated.  
 */
Vector* initVector(Vector* v, size_t typeSize, size_t initSize,
                   const void* contents, size_t num,
                   void* (*initializer)(void*, const void*), 
                   void (*deInitializer)(void*)) {
  initSize = initSize < 2 ? _VECTOR_DEFAULT_INIT_SIZE : initSize;
  initSize = initSize > num ? initSize: num + 1;
  v->arr = malloc(typeSize * initSize);
  v->_arrSize = initSize;
  v->_deInitializer = deInitializer;
  v->_typeSize = typeSize;
  v->length = 0;

  VectorErr verr;
  Vector_catPrimitive(v, contents, num, &verr);

  return v;
}

void destroyVector(Vector** v) {
  deinitVector(*v);
  free(*v);
  *v = NULL;
}

/**
 * Deinitializes the vector into a undefined state. It must be initialized again
 * to be reused.
 */
void deinitVector(Vector* v) {
  Vector_clear(v);
  free(v->arr);
}

extern Vector* newByteVector(size_t initSize, const char *contents, size_t num);
extern Vector* initByteVector(Vector* v, size_t initSize, const char* contents, size_t num);
extern Vector* newDoubleVector(const double* contents, size_t num);
extern Vector* initDoubleVector(Vector* v, const char* contents, size_t num);
extern Vector* newIntVector(const int* contents, size_t num);

/**
 * We want the pointer to the [element] but remember that it isn't the pointer
 * that is added to the vector. The element value itself will be copied over.
 * Returned is the address to it's location in the Vector 
 * (same as VectorPtrAt())
 */
void* Vector_add(Vector* v, const void* element, VectorErr* e) {
  _Vector_resize(v, 1, e);
  if (*e == E_NOMEMS) {
    return NULL;
  }

  void* arrayPosition = _Vector_appendCopy(v, element);
  _Vector_appendNull(v);

  return arrayPosition;
}

/**
 * Copy one Vector onto the end of another. Vector [other] gets appended to
 * Vector [v] in this case.
 */
Vector* Vector_cat(Vector* v, const Vector* other, VectorErr* e) {
  if (v->_typeSize != other->_typeSize) {
    *e = E_INCOMPATIBLE_TYPES;
    return v;
  }

  return Vector_catPrimitive(v, other->arr, other->length, e);
}

Vector* Vector_catPrimitive(Vector* v, const void* arr, size_t num, VectorErr* e) {
  if (num > 0) {
    _Vector_resize(v, num, e);
    if (*e == E_NOMEMS) {
      return v;
    }

    for (int i = 0; i < num; ++i) {
      _Vector_appendCopy(v, arr + i * v->_typeSize);
    }

    // For compatibility with primitive array functions, always append a NULL
    // value.
    _Vector_appendNull(v);
  }
  
  return v;
}

Vector* Vector_clear(Vector* v) {
  VectorErr verr;
  if (v->_deInitializer) {
    for (int i = 0; i < v->length; ++i) {
      v->_deInitializer((void*) Vector_ptrAt(v, i, &verr));
    }
  }

  v->length = 0; // I cheat LOL
  _Vector_appendNull(v);
  
  return v;
}

void Vector_forEach(const Vector* v, 
                   void (*action)(void* itemPtr, int index)) {
  VectorErr e;
  for (int i = 0; i < v->length; ++i) {
    void* item = Vector_ptrAt(v, i, &e);
    action(item, i);
  }
}

/**
 * Returns a pointer to the specified [index] value.
 * Error if index is out of range.
 */
void* Vector_ptrAt(const Vector* v, int index, VectorErr* e) { 
  if (index >= v->length || index < 0) {
    *e = E_RANGE;
    return v->arr;
  } 

  return _Vector_calcPtrAt(v, index);
}

void* _Vector_appendCopy(Vector* v, const void* element) {
  void* arrayPosition = _Vector_calcDanglingPtr(v);
  if (v->_copyInitializer) {
    v->_copyInitializer(arrayPosition, element);
  } else {
    memcpy(arrayPosition, element, v->_typeSize);
  }
  ++(v->length);
  return arrayPosition;
}

void _Vector_appendNull(const Vector* v) {
  void* nilly = malloc(v->_typeSize);

  memcpy(_Vector_calcDanglingPtr(v), nilly, v->_typeSize);
  free(nilly);
}

void* _Vector_calcPtrAt(const Vector* v, int index) {
  return ((char*) v->arr) + index * v->_typeSize;
}

void* _Vector_calcDanglingPtr(const Vector* v) {
  return _Vector_calcPtrAt(v, v->length);
}

void _Vector_resize(Vector *v, size_t numAdded, VectorErr *e) {
  if (v->_arrSize <= v->length + numAdded) {
    v->_arrSize = v->length + numAdded + 1 /* Null element */;
    v->_arrSize *= 2; // For good measure.

    void* newMems = realloc(v->arr, v->_arrSize * v->_typeSize);
    if (newMems == NULL) {
      v->_arrSize = v->length;
      *e = E_NOMEMS;
    } else {
      v->arr = newMems;
    }
  }
}
