#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#ifndef __BCC__

#include <stddef.h>
#include <stdbool.h>

#include "systemError.h"

typedef struct SingleLinkedNode SingleLinkedNode;

typedef struct LinkedList {
  SingleLinkedNode* firstNode;
  SingleLinkedNode* lastNode;
  size_t length;

  void* (*_copyInitializer)(void*, const void*, SystemErr*);
  void (*_deInitializer)(void*);
  size_t _typeSize;
} LinkedList;

struct SingleLinkedNode {
  void* data;
  SingleLinkedNode* next;
};

typedef enum LLErr {
  LL_E_CLEAR,
  LL_E_NOT_FOUND
} LLErr;

LinkedList* initLinkedList(LinkedList*, size_t, void* (*)(void*, const void*, SystemErr*),
                           void (*)(void*));
LinkedList* initLinkedListCp(LinkedList*, const LinkedList*, SystemErr*);
void deinitLinkedList(LinkedList*);

SingleLinkedNode* initSingleLinkedNode(SingleLinkedNode*, const void*, size_t,
                                       void* (*)(void*, const void*, SystemErr*),
                                       SystemErr*);
SingleLinkedNode* initSingleLinkedNode_empty(SingleLinkedNode* node,
                                             size_t typeSize,
                                             SystemErr* se);
void deinitSingleLinkedNode(SingleLinkedNode*, size_t typeSize, void (*)(void*));

void LinkedList_append(LinkedList*, const void* data, SystemErr*);
void* LinkedList_appendEmpty(LinkedList* list, SystemErr* se);
void LinkedList_prepend(LinkedList*, const void* data, SystemErr*);

void* LinkedList_first(const LinkedList*);

void* LinkedList_last(const LinkedList*);
void LinkedList_clear(LinkedList*);

void LinkedList_removeFirst(LinkedList*);
void LinkedList_removeLast(LinkedList*);

void* LinkedList_find(LinkedList* list, void* dataToFind,
                      bool (*cmp)(void* dataToFind, void* itemData), LLErr* le);

#endif
#endif
