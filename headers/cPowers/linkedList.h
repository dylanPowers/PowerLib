#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct LinkedList LinkedList;
typedef struct SingleLinkedNode SingleLinkedNode;

LinkedList* newLinkedList(int, void* (*)(void*, const void*), void (*)(void*));
LinkedList* initLinkedList(LinkedList*, int, void* (*)(void*, const void*), 
                           void (*)(void*));
void destroyLinkedList(LinkedList**);

SingleLinkedNode* newSingleLinkedNode(const void*, int, 
                                      void* (*)(void*, const void*));
SingleLinkedNode* initSingleLinkedNode(SingleLinkedNode*, const void*, int, 
                                       void* (*)(void*, const void*));
void destroySingleLinkedNode(SingleLinkedNode**, void (*)(void*));

void LinkedList_prepend(LinkedList*, const void* data);
void LinkedList_append(LinkedList*, const void* data);

void* LinkedList_first(const LinkedList*);
void* LinkedList_last(const LinkedList*);

void LinkedList_clear(LinkedList*);
void LinkedList_removeFirst(LinkedList*);
void LinkedList_removeLast(LinkedList*);

void LinkedList_forEach(const LinkedList*, void (*)(void*));

#endif
