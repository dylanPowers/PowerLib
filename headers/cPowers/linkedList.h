#include <stddef.h>

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct SingleLinkedNode SingleLinkedNode;

typedef struct LinkedList {
  SingleLinkedNode* firstNode;
  int length;

  void* (*_copyInitializer)(void*, const void*);
  void (*_deInitializer)(void*);
  size_t _typeSize;
} LinkedList;

struct SingleLinkedNode {
  void* data;
  SingleLinkedNode* next;
};

//LinkedList* newLinkedList(int, void* (*)(void*, const void*), void (*)(void*));
LinkedList* initLinkedList(LinkedList*, size_t, void* (*)(void*, const void*),
                           void (*)(void*));
LinkedList* initLinkedListCp(LinkedList*, const LinkedList*);
//void destroyLinkedList(LinkedList**);
void deinitLinkedList(LinkedList*);

//SingleLinkedNode* newSingleLinkedNode(const void*, int,
//                                      void* (*)(void*, const void*));
SingleLinkedNode* initSingleLinkedNode(SingleLinkedNode*, const void*, size_t,
                                       void* (*)(void*, const void*));
//void destroySingleLinkedNode(SingleLinkedNode**, void (*)(void*));
void deinitSingleLinkedNode(SingleLinkedNode*, void (*)(void*));

void LinkedList_prepend(LinkedList*, const void* data);
void LinkedList_append(LinkedList*, const void* data);

void* LinkedList_first(const LinkedList*);
void* LinkedList_last(const LinkedList*);

void LinkedList_clear(LinkedList*);
void LinkedList_removeFirst(LinkedList*);
void LinkedList_removeLast(LinkedList*);

//void LinkedList_forEach(const LinkedList*, void (*)(void*));

#endif
