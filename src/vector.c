#include "vector.h"

#include "string.h" // memcpy() has to do with strings apparently

#define _VECTOR_DEFAULT_INIT_SIZE 16

void* _Vector_appendCopy(Vector*, const void*);
void _Vector_appendNull(const Vector*);
void* _Vector_calcPtrAt(const Vector*, size_t);
void* _Vector_calcDanglingPtr(const Vector*);
void _Vector_reinit(Vector* v, size_t typeSize, size_t* initSize, VectorErr* e);
void _Vector_resize(Vector *, size_t, VectorErr*);

/**
 * @errors  V_E_NOMEMS
 */
Vector* initVector(Vector* v, size_t typeSize,
                   void* (*initializer)(void*, const void*, void*),
                   void (*deInitializer)(void*), VectorErr* e) {
  return initVectorAdvanced(v, typeSize, _VECTOR_DEFAULT_INIT_SIZE, NULL, 0,
                            initializer, deInitializer, e);
}

/**
 * @errors  V_E_NOMEMS
 */
Vector* initVectorCp(Vector* v, const Vector* copy, VectorErr* e) {
  return initVectorAdvanced(v, copy->_typeSize, copy->_arrSize,
                            copy->arr, copy->length, copy->_copyInitializer,
                            copy->_deInitializer, e);
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
 * number of elements in [contents]. [initializer] is the function used to
 * copy elements into the vector. [deInitializer] is the function that is
 * used to dispose of elements in the vector when they are removed.
 * @error V_E_NOMEMS
 */
Vector* initVectorAdvanced(Vector* v, size_t typeSize, size_t initSize,
                           const void* contents, size_t num,
                           void* (*cpInitializer)(void*, const void*, void*),
                           void (*deInitializer)(void*), VectorErr* e) {
  initSize = initSize < 2 ? _VECTOR_DEFAULT_INIT_SIZE : initSize;
  initSize = initSize > num ? initSize: num + 1; // +1 remember null end

  if (v->arr == NULL) {
    v->arr = malloc(typeSize * initSize);
    if (v->arr == NULL) {
      *e = V_E_NOMEMS;
    }
  } else {
    _Vector_reinit(v, typeSize, &initSize, e);
  }

  if (!*e) {
    v->_arrSize = initSize;
    v->_copyInitializer = cpInitializer;
    v->_deInitializer = deInitializer;
    v->_typeSize = typeSize;
    v->length = 0;

    _Vector_appendNull(v);
    Vector_catPrimitive(v, contents, num, e);
  }

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

extern Vector* initByteVector(Vector* v, size_t initSize, const char* contents,
                              size_t num, VectorErr*);
extern Vector* initDoubleVector(Vector* v, const char* contents, size_t num,
                                VectorErr*);

/**
 * We want the pointer to the [element] but remember that it isn't the pointer
 * that is added to the vector. The element value itself will be copied over.
 * Returned is the address to it's location in the Vector 
 * (same as VectorPtrAt())
 * @error  V_E_NOMEMS
 */
void* Vector_add(Vector* v, const void* element, VectorErr* e) {
  _Vector_resize(v, 1, e);
  if (*e == V_E_NOMEMS) {
    return NULL;
  }

  void* arrayPosition = _Vector_appendCopy(v, element);
  _Vector_appendNull(v);

  return arrayPosition;
}

/**
 * Returns a pointer to the specified [index] value.
 * Error if index is out of range.
 * @error  V_E_RANGE
 */
void* Vector_at(const Vector* v, size_t index, VectorErr* e) {
  if (index >= v->length || index < 0) {
    *e = V_E_RANGE;
    return v->arr;
  }

  return _Vector_calcPtrAt(v, index);
}

/**
 * Copy one Vector onto the end of another. Vector [other] gets appended to
 * Vector [v] in this case.
 * @error  V_E_INCOMPATIBLE_TYPES
 */
Vector* Vector_cat(Vector* v, const Vector* other, VectorErr* e) {
  if (v->_typeSize != other->_typeSize) {
    *e = V_E_INCOMPATIBLE_TYPES;
    return v;
  }

  return Vector_catPrimitive(v, other->arr, other->length, e);
}

/**
 * @error  V_E_NOMEMS
 */
Vector* Vector_catPrimitive(Vector* v, const void* arr, size_t num, VectorErr* e) {
  if (num > 0) {
    _Vector_resize(v, num, e);
    if (*e == V_E_NOMEMS) {
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
    VectorErr eIgnore = V_E_CLEAR;
    for (int i = 0; i < v->length; ++i) {
      v->_deInitializer((void*) Vector_at(v, i, &eIgnore));
    }
  }

  v->length = 0; // I cheat LOL
  _Vector_appendNull(v);
  
  return v;
}

/**
 * @error  V_E_EMPTY
 */
void* Vector_last(Vector* v, VectorErr* e) {
  if (v->length == 0) {
    *e = V_E_EMPTY;
  } else {
    size_t last = v->length - 1;
    VectorErr eIgnore = V_E_CLEAR;
    return Vector_at(v, last, &eIgnore);
  }

  return NULL;
}

void Vector_removeLast(Vector* v) {
  VectorErr e = V_E_CLEAR;
  void* lastEl = Vector_last(v, &e);
  if (!e) {
    if (v->_deInitializer) {
      v->_deInitializer(lastEl);
    }

    v->length--;
  }
}

/**
 * Copies a reversed version of [v] into [reversed]
 * @error  V_E_NOMEMS
 */
void Vector_reverse(const Vector* v, Vector* reversed, VectorErr* e) {
  for (size_t i = 0; i < v->length && !*e; ++i) {
    Vector_add(reversed, _Vector_calcPtrAt(v, v->length - 1 - i), e);
  }
}

void* _Vector_appendCopy(Vector* v, const void* element) {
  void* arrayPosition = _Vector_calcDanglingPtr(v);
  if (v->_copyInitializer) {
    int error = 0;
    v->_copyInitializer(arrayPosition, element, &error);
  } else {
    memcpy(arrayPosition, element, v->_typeSize);
  }
  ++(v->length);
  return arrayPosition;
}

void _Vector_appendNull(const Vector* v) {
  void* nilly = malloc(v->_typeSize);
  for (int i = 0; i < v->_typeSize; ++i) {
    *((char*) nilly + i) = (char) 0;
  }

  memcpy(_Vector_calcDanglingPtr(v), nilly, v->_typeSize);
  free(nilly);
}

void* _Vector_calcPtrAt(const Vector* v, size_t index) {
  return ((char*) v->arr) + index * v->_typeSize;
}

void* _Vector_calcDanglingPtr(const Vector* v) {
  return _Vector_calcPtrAt(v, v->length);
}

/**
 * @error V_E_NOMEMS
 */
void _Vector_reinit(Vector* v, size_t typeSize, size_t* initSize, VectorErr* e) {
  if (v->_typeSize * v->_arrSize < typeSize * (*initSize)) {
    void* newMems = realloc(v->arr, typeSize * (*initSize));
    if (newMems == NULL) {
      *e = V_E_NOMEMS;
    }
  } else {
    *initSize = v->_typeSize * v->_arrSize / typeSize;
  }
}

/**
 * @error  V_E_NOMEMS
 */
void _Vector_resize(Vector *v, size_t numAdded, VectorErr* e) {
  if (v->_arrSize <= v->length + numAdded) {
    v->_arrSize = v->length + numAdded + 1 /* Null element */;
    v->_arrSize *= 2; // For good measure.

    void* newMems = realloc(v->arr, v->_arrSize * v->_typeSize);
    if (newMems == NULL) {
      v->_arrSize = v->length;
      *e = V_E_NOMEMS;
    } else {
      v->arr = newMems;
    }
  }
}
