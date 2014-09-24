#include "vector.h"

#include "string.h" // memcpy() has to do with strings apparently

#define _VECTOR_DEFAULT_INIT_SIZE 16

void* _Vector_appendCopy(Vector*, const void*);
void _Vector_appendNull(const Vector*);
void* _Vector_calcPtrAt(const Vector*, size_t);
void* _Vector_calcDanglingPtr(const Vector*);
void _Vector_resize(Vector *, size_t);

Vector* initVector(Vector* v, size_t typeSize,
                   void* (*initializer)(void*, const void*),
                   void (*deInitializer)(void*)) {
  return initVectorAdvanced(v, typeSize, _VECTOR_DEFAULT_INIT_SIZE, NULL, 0,
                            initializer, deInitializer);
}

Vector* initVectorCp(Vector* v, const Vector* copy) {
  return initVectorAdvanced(v, copy->_typeSize, copy->_arrSize,
                            copy->arr, copy->length, copy->_copyInitializer,
                            copy->_deInitializer);
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
Vector* initVectorAdvanced(Vector* v, size_t typeSize, size_t initSize,
                           const void* contents, size_t num,
                           void* (*initializer)(void*, const void*),
                           void (*deInitializer)(void*)) {
  initSize = initSize < 2 ? _VECTOR_DEFAULT_INIT_SIZE : initSize;
  initSize = initSize > num ? initSize: num + 1;
  v->arr = malloc(typeSize * initSize);
  v->_arrSize = initSize;
  v->_copyInitializer = initializer;
  v->_deInitializer = deInitializer;
  v->_typeSize = typeSize;
  v->length = 0;

  Vector_catPrimitive(v, contents, num);

  return v;
}

/**
 * Deinitializes the vector into a undefined state. It must be initialized again
 * to be reused.
 */
void deinitVector(Vector* v) {
  Vector_clear(v);
  free(v->arr);
}

extern Vector* initByteVector(Vector* v, size_t initSize, const char* contents, size_t num);
extern Vector* initDoubleVector(Vector* v, const char* contents, size_t num);

/**
 * We want the pointer to the [element] but remember that it isn't the pointer
 * that is added to the vector. The element value itself will be copied over.
 * Returned is the address to it's location in the Vector 
 * (same as VectorPtrAt())
 */
void* Vector_add(Vector* v, const void* element) {
  _Vector_resize(v, 1);
  if (v->e == V_E_NOMEMS) {
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
Vector* Vector_cat(Vector* v, const Vector* other) {
  if (v->_typeSize != other->_typeSize) {
    v->e = V_E_INCOMPATIBLE_TYPES;
    return v;
  }

  return Vector_catPrimitive(v, other->arr, other->length);
}

Vector* Vector_catPrimitive(Vector* v, const void* arr, size_t num) {
  if (num > 0) {
    _Vector_resize(v, num);
    if (v->e == V_E_NOMEMS) {
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
  if (v->_deInitializer) {
    for (int i = 0; i < v->length; ++i) {
      v->_deInitializer((void*) Vector_at(v, i));
    }
  }

  v->length = 0; // I cheat LOL
  _Vector_appendNull(v);
  
  return v;
}

void* Vector_last(Vector* v) {
  if (v->length == 0) {
    v->e = V_E_EMPTY;
  } else {
    size_t last = v->length - 1;
    return Vector_at(v, last);
  }

  return NULL;
}

/**
 * Returns a pointer to the specified [index] value.
 * Error if index is out of range.
 */
void* Vector_at(Vector* v, size_t index) {
  if (index >= v->length || index < 0) {
    v->e = V_E_RANGE;
    return v->arr;
  } 

  return _Vector_calcPtrAt(v, index);
}

void Vector_removeLast(Vector* v) {
  void* lastEl = Vector_last(v);
  if (!v->e) {
    if (v->_deInitializer) {
      v->_deInitializer(lastEl);
    }

    v->length--;
  }
}

void Vector_reverse(const Vector* v, Vector* reversed) {
  for (size_t i = v->length - 1; i >= 0; --i) {
    Vector_add(reversed, _Vector_calcPtrAt(v, i));
  }
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

void* _Vector_calcPtrAt(const Vector* v, size_t index) {
  return ((char*) v->arr) + index * v->_typeSize;
}

void* _Vector_calcDanglingPtr(const Vector* v) {
  return _Vector_calcPtrAt(v, v->length);
}

void _Vector_resize(Vector *v, size_t numAdded) {
  if (v->_arrSize <= v->length + numAdded) {
    v->_arrSize = v->length + numAdded + 1 /* Null element */;
    v->_arrSize *= 2; // For good measure.

    void* newMems = realloc(v->arr, v->_arrSize * v->_typeSize);
    if (newMems == NULL) {
      v->_arrSize = v->length;
      v->e = V_E_NOMEMS;
    } else {
      v->arr = newMems;
    }
  }
}
