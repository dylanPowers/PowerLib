#include <stddef.h>

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "systemError.h"

typedef struct SingleLinkedNode SingleLinkedNode;

typedef struct LinkedList {
  SingleLinkedNode* firstNode;

  void* (*_copyInitializer)(void*, const void*, SystemErr*);
  void (*_deInitializer)(void*);
  size_t _typeSize;
} LinkedList;

struct SingleLinkedNode {
  void* data;
  SingleLinkedNode* next;
};

LinkedList* initLinkedList(LinkedList*, size_t, void* (*)(void*, const void*, SystemErr*),
                           void (*)(void*));
LinkedList* initLinkedListCp(LinkedList*, const LinkedList*, SystemErr*);
void deinitLinkedList(LinkedList*);

SingleLinkedNode* initSingleLinkedNode(SingleLinkedNode*, const void*, size_t,
                                       void* (*)(void*, const void*, SystemErr*),
                                       SystemErr*);
void deinitSingleLinkedNode(SingleLinkedNode*, void (*)(void*));

void LinkedList_prepend(LinkedList*, const void* data, SystemErr*);
void LinkedList_append(LinkedList*, const void* data, SystemErr*);

void* LinkedList_first(const LinkedList*);
void* LinkedList_last(const LinkedList*);

void LinkedList_clear(LinkedList*);
void LinkedList_removeFirst(LinkedList*);
void LinkedList_removeLast(LinkedList*);

#endif
