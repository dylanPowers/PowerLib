#include "vector.h"

#include "string.h" // memcpy() has to do with strings apparently


/**
 * @errors  S_E_NOMEMS
 */
Vector* initVector(Vector* v, size_t typeSize,
                   void* (*initializer)(void*, const void*, Err*),
                   void (*deInitializer)(void*), Err* se) {
  return initVectorAdvanced(v, typeSize, _VECTOR_DEFAULT_INIT_SIZE, NULL, 0,
                            initializer, deInitializer, se);
}

/**
 * @errors  S_E_NOMEMS
 */
Vector* initVectorCp(Vector* v, const Vector* copy, Err* se) {
  return initVectorAdvanced(v, copy->_typeSize, copy->_arrSize,
                            copy->arr, copy->length, copy->_copyInitializer,
                            copy->_deInitializer, se);
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
 * @error S_E_NOMEMS
 */
Vector* initVectorAdvanced(Vector* v, size_t typeSize, size_t initSize,
                           const void* contents, size_t num,
                           void* (*cpInitializer)(void*, const void*, Err*),
                           void (*deInitializer)(void*), SystemErrNoMems* se) {
  initSize = initSize < 2 ? _VECTOR_DEFAULT_INIT_SIZE : initSize;
  initSize = initSize > num ? initSize: num + 1; // +1 remember null end

  v->arr = malloc(typeSize * initSize);
  if (v->arr == NULL) {
    se->any = true;
    sprintf(se->msg, "initVector: No more memory available");
  }

  if (!se->any) {
    v->_arrSize = initSize;
    v->_copyInitializer = cpInitializer;
    v->_deInitializer = deInitializer;
    v->_typeSize = typeSize;
    v->length = 0;

    _Vector_appendNull(v);
    Vector_catPrimitive(v, contents, num, se);
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
  v->arr = NULL;
}


Vector* initByteVector(Vector* v, size_t initSize,
                              const char* contents, size_t num, Err* e) {
  return initVectorAdvanced(v, sizeof(char), initSize, contents, num, NULL, NULL, e);
}

Vector* initIntVector(Vector* v, const char* contents, size_t num, Err* se) {
  return initVectorAdvanced(v, sizeof(int), 0, contents, num, NULL, NULL, se);
}

Vector* initDoubleVector(Vector* v, const char* contents, size_t num,
                                Err* e) {
  return initVectorAdvanced(v, sizeof(double), 0, contents, num, NULL, NULL, e);
}
/**
 * We want the pointer to the [element] but remember that it isn't the pointer
 * that is added to the vector. The element value itself will be copied over.
 * Returned is the address to it's location in the Vector 
 * (same as VectorPtrAt())
 * @error  S_E_NOMEMS
 */
void* Vector_add(Vector* v, const void* element, SystemErrNoMems* se) {
  void* arrayPosition;

  _Vector_resize(v, 1, se);
  if (se->any) {
    return NULL;
  }

  arrayPosition = _Vector_appendCopy(v, element, se);
  _Vector_appendNull(v);

  return arrayPosition;
}

/**
 * Adds an element to the vector that has been initialized to 0. The returned
 * memory location must be initialized or you will later be very sorry.
 * @error S_E_NOMEMS
 */
void* Vector_addEmpty(Vector* v, Err* se) {
  void* mems;
  _Vector_resize(v, 1, se);
  if (se->any) return NULL;

  v->length++;
  _Vector_appendNull(v);
  mems = _Vector_calcPtrAt(v, v->length - 1); // Last element was previously nulled
  return mems;
}

/**
 * Returns a pointer to the specified [index] value.
 * Error if index is out of range.
 * @error  V_E_RANGE
 */
void* Vector_at(const Vector* v, size_t index, VectorErrRange* e) {
  if (index >= v->length) {
    e->any = true;
    sprintf(e->msg, "Range error: Index %d out of range %d", index, v->length);
    return v->arr;
  }

  return _Vector_calcPtrAt(v, index);
}

/**
 * Copy one Vector onto the end of another. Vector [other] gets appended to
 * Vector [v] in this case.
 * @error  V_E_INCOMPATIBLE_TYPES
 * @error S_E_NOMEMS
 */
Vector* Vector_cat(Vector* v, const Vector* other, VectorErrIncompatibleTypes* e, SystemErrNoMems* se) {
  if (v->_typeSize != other->_typeSize) {
    e->any = true;
    sprintf(e->msg, "Vector types are incompatible. Typesize %d != %d", 
      v->_typeSize, other->_typeSize);
    return v;
  }

  return Vector_catPrimitive(v, other->arr, other->length, se);
}

/**
 * @error  S_E_NOMEMS
 */
Vector* Vector_catPrimitive(Vector* v, const void* arr, size_t num, SystemErrNoMems* se) {
  int i;

  if (num > 0) {
    _Vector_resize(v, num, se);
    if (se->any) {
      return v;
    }

    for (i = 0; i < num; ++i) {
      _Vector_appendCopy(v, ((char*) arr) + i * v->_typeSize, se);
    }

    // For compatibility with primitive array functions, always append a NULL
    // value.
    _Vector_appendNull(v);
  }
  
  return v;
}

Vector* Vector_clear(Vector* v) {
  int i;
  VectorErrRange eIgnore;
  void* el;
  eIgnore.any = false;
  if (v->_deInitializer) {
    for (i = 0; i < v->length; ++i) { 
      el = (void*) Vector_at(v, i, &eIgnore);
      v->_deInitializer(el);
    }
  }

  v->length = 0; // I cheat LOL
  _Vector_appendNull(v);
  
  return v;
}

/**
 * @error  V_E_EMPTY
 */
void* Vector_last(Vector* v, VectorErrEmpty* e) {
  VectorErrRange eIgnore;
  eIgnore.any = false;
  if (v->length == 0) {
    e->any = 1;
    sprintf(e->msg, "Empty vector");
  } else {
    size_t last = v->length - 1;
    return Vector_at(v, last, &eIgnore);
  }

  return NULL;
}

void Vector_removeLast(Vector* v) {
  VectorErrEmpty e;
  void* lastEl;
  e.any = false;
  lastEl = Vector_last(v, &e);
  if (!e.any) {
    if (v->_deInitializer) {
      v->_deInitializer(lastEl);
    }

    v->length--;
    _Vector_appendNull(v);
  }
}

/**
 * Copies a reversed version of [v] into [reversed]
 * @error  S_E_NOMEMS
 */
void Vector_reverse(const Vector* v, Vector* reversed, Err* se) {
  int i;
  for (i = 0; i < v->length && !se->any; ++i) {
    Vector_add(reversed, _Vector_calcPtrAt(v, v->length - 1 - i), se);
  }
}

void* _Vector_appendCopy(Vector* v, const void* element, Err* se) {
  void* arrayPosition = _Vector_calcDanglingPtr(v);
  if (v->_copyInitializer) {
    memset(arrayPosition, 0, v->_typeSize);
    v->_copyInitializer(arrayPosition, element, se);
  } else {
    memcpy(arrayPosition, element, v->_typeSize);
  }
  ++(v->length);
  return arrayPosition;
}

void _Vector_appendNull(const Vector* v) {
  memset(_Vector_calcDanglingPtr(v), 0, v->_typeSize);
}

void* _Vector_calcPtrAt(const Vector* v, size_t index) {
  return ((char*) v->arr) + index * v->_typeSize;
}

void* _Vector_calcDanglingPtr(const Vector* v) {
  return _Vector_calcPtrAt(v, v->length);
}

/**
 * @error S_E_NOMEMS
 */
void _Vector_reinit(Vector* v, size_t typeSize, size_t* initSize, SystemErrNoMems* se) {
  if (v->_typeSize * v->_arrSize < typeSize * (*initSize)) {
    void* newMems = realloc(v->arr, typeSize * (*initSize));
    if (newMems == NULL) {
      se->any = true;
      sprintf(se->msg, "Vector reinit: No more memory available");
    }
  } else {
    *initSize = v->_typeSize * v->_arrSize / typeSize;
  }
}

/**
 * @error  S_E_NOMEMS
 */
void _Vector_resize(Vector *v, size_t numAdded, SystemErrNoMems* se) {
  void* newMems;
  if (v->_arrSize <= v->length + numAdded) {
    v->_arrSize = v->length + numAdded + 1 /* Null element */;
    v->_arrSize *= 2; // For good measure.

    newMems = realloc(v->arr, v->_arrSize * v->_typeSize);
    if (newMems == NULL) {
      v->_arrSize = v->length;
      se->any = true;
      sprintf(se->msg, "Vector resize: No more memory available");
    } else {
      v->arr = newMems;
    }
  }
}
