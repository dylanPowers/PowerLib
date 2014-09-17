#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct LinkedList LinkedList;
typedef struct SingleLinkedNode SingleLinkedNode;

LinkedList* newLinkedList(void* (*)(void*), void (*)(void**));
LinkedList* initLinkedList(LinkedList*, void* (*)(void*), void (*)(void**));
void destroyLinkedList(LinkedList**);

SingleLinkedNode* newSingleLinkedNode(void*, void* (*)(void*));
SingleLinkedNode* initSingleLinkedNode(SingleLinkedNode*, void*, void* (*)(void*));
void destroySingleLinkedNode(SingleLinkedNode**, void (*)(void**));

void LinkedList_prepend(LinkedList*, void* data);
void LinkedList_append(LinkedList*, void* data);

void* LinkedList_first(const LinkedList*);
void* LinkedList_last(const LinkedList*);

void LinkedList_clear(LinkedList*);
void LinkedList_removeFirst(LinkedList*);
void LinkedList_removeLast(LinkedList*);

void LinkedList_forEach(const LinkedList*, void (*)(void*));

#endif
