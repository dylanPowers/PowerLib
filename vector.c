#include "vector.h"

/**
 * This function allocates memory for a new Vector and appropriately
 * initializes it. 
 * Know that [contents] is an optional argument. Set it to NULL or
 * [num] to 0 and it will be ignored.
 */
Vector* newVector(int typeSize, const void* contents, int num, 
                  void (*deconstructor)(void**)) {
  return newVectorWithSize(typeSize, 0, contents, num, deconstructor);
}

Vector* newVectorWithSize(int typeSize, int initSize, 
                          const void* contents, int num,
                          void (*deconstructor)(void**)) {
  Vector* vec = (Vector*) malloc(sizeof(Vector));
  return initVector(vec, typeSize, initSize, contents, num, deconstructor);
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
Vector* initVector(Vector* v, int typeSize, int initSize, 
                const void* contents, int num, void (*deconstructor)(void**)) {
  initSize = initSize < 2 ? _VECTOR_DEFAULT_INIT_SIZE : initSize;
  initSize = initSize > num ? initSize: num + 1;
  v->arr = malloc(typeSize * initSize);
  v->_arrSize = initSize;
  v->_destruct = deconstructor;
  v->_typeSize = typeSize;
  v->length = 0;

  VectorErr verr;
  VectorCatPrimitive(v, contents, num, &verr);

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
  VectorClear(v);
  free(v->arr);
}

extern Vector* newByteVector(int initSize, const char *contents, int num);
extern Vector* initByteVector(Vector* v, int initSize, const char* contents, int num);
extern Vector* newDoubleVector(const double* contents, int num);
extern Vector* initDoubleVector(Vector* v, const char* contents, int num);
extern Vector* newIntVector(const int* contents, int num);
extern Vector* newPointerVector(const void* contents, int num, 
                                void (*deconstructor)(void**));
extern Vector* initPointerVector(Vector* v, const void *contents, int num, 
                                 void (*deconstructor)(void**));

/**
 * We want the pointer to the [element] but remember that it isn't the pointer
 * that is added to the vector. The element value itself will be copied over.
 * Returned is the address to it's location in the Vector 
 * (same as VectorPtrAt())
 */
void* VectorAdd(Vector* v, const void* element, VectorErr* e) {
  if (v->_arrSize < v->length + 1 + 1) {
    v->_arrSize *= 2;
    v->arr = realloc(v->arr, v->_arrSize * v->_typeSize);
  }

  void* arrayPosition = _VectorCalcDanglingPtr(v);
  memcpy(arrayPosition, element, v->_typeSize); // Owned. Get assigned.
  ++(v->length);

  _VectorAppendNull(v);

  return arrayPosition;
}


/**
 * Copy one Vector onto the end of another. Vector [other] gets appended to
 * Vector [v] in this case.
 */
Vector* VectorCat(Vector* v, const Vector* other, VectorErr* e) {
  if (v->_typeSize != other->_typeSize) {
    *e = E_INCOMPATIBLE_TYPES;
    return v;
  }

  return VectorCatPrimitive(v, other->arr, other->length, e);
}

Vector* VectorCatPrimitive(Vector* v, const void* arr, int num, VectorErr* e) {
  if (num > 0) {

    if (v->_arrSize < v->length + num + 1) {
      v->_arrSize = v->length + num + 1;
      v->arr = realloc(v->arr, v->_arrSize * v->_typeSize);
      if (v->arr == NULL) {
        v->_arrSize = v->length;
        *e = E_NOMEMS;
        return v;
      }
    }

    void* arrayPosition = _VectorCalcDanglingPtr(v);
    memcpy(arrayPosition, arr, v->_typeSize * num);
    v->length += num;

    // For compatibility with primitive array functions, always append a NULL
    // value.
    _VectorAppendNull(v); 
  }
  
  return v;
}

Vector* VectorClear(Vector* v) {
  VectorErr verr;
  if (v->_destruct) {
    for (int i = 0; i < v->length; ++i) {
      v->_destruct((void**) VectorPtrAt(v, i, &verr));
    }
  }

  v->length = 0; // I cheat LOL
  _VectorAppendNull(v);
  
  return v;
}

/**
 * Returns a pointer to the specified [index] value.
 */
void* VectorPtrAt(const Vector* v, int index, VectorErr* e) { 
  if (index >= v->length || index < 0) {
    *e = E_RANGE;
    return v->arr;
  } 

  return _VectorCalcPtrAt(v, index);
}

void _VectorAppendNull(const Vector* v) {
  void* nilly = 0;
  int numBytes = sizeof(void*);

  // It's safe to assume that if the object type stored is larger than a pointer
  // then it's not of a primitive type and an appended NULL value would be
  // meaningless

  memcpy(_VectorCalcDanglingPtr(v), &nilly, 
         v->_typeSize < numBytes ? v->_typeSize : numBytes);
}

extern void* _VectorCalcPtrAt(const Vector *v, int index);
extern void* _VectorCalcDanglingPtr(const Vector *v);