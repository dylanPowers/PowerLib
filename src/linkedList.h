#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct LinkedList LinkedList;
typedef struct SingleLinkedNode SingleLinkedNode;

LinkedList* newLinkedList(int, void* (*)(void*), void (*)(void*));
LinkedList* initLinkedList(LinkedList*, int, void* (*)(void*), void (*)(void*));
void destroyLinkedList(LinkedList**);

SingleLinkedNode* newSingleLinkedNode(void*, int, void* (*)(void*));
SingleLinkedNode* initSingleLinkedNode(SingleLinkedNode*, void*, int, void* (*)(void*));
void destroySingleLinkedNode(SingleLinkedNode**, void (*)(void*));

void LinkedList_prepend(LinkedList*, void* data);
void LinkedList_append(LinkedList*, void* data);

void* LinkedList_first(const LinkedList*);
void* LinkedList_last(const LinkedList*);

void LinkedList_clear(LinkedList*);
void LinkedList_removeFirst(LinkedList*);
void LinkedList_removeLast(LinkedList*);

void LinkedList_forEach(const LinkedList*, void (*)(void*));

#endif
