#include "linkedList.h"

#include "stdlib.h"
#include "string.h"

SingleLinkedNode* _LinkedList_lastNode(const LinkedList*);
void _LinkedList_nullifyLast(LinkedList*, SingleLinkedNode*);

struct LinkedList {
  SingleLinkedNode* firstNode;
  int length;

  void* (*_copyConstructor)(void*);
  void (*_destructor)(void**);
};

struct SingleLinkedNode {
  void* data;
  SingleLinkedNode* next;
};

LinkedList* newLinkedList(void* (*copyConstructor)(void*), 
                          void (*destructor)(void**)) {
  LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
  return initLinkedList(list, copyConstructor, destructor);
}

LinkedList* initLinkedList(LinkedList* list, void* (*copyConstructor)(void*), 
                           void (*destructor)(void**)) {
  list->firstNode = NULL;
  list->length = 0;
  list->_copyConstructor = copyConstructor;
  list->_destructor = destructor;
  return list;
}

void destroyLinkedList(LinkedList** listPtr) {
  LinkedList_clear(*listPtr);
  free(*listPtr);
  *listPtr = NULL;
}

SingleLinkedNode* newSingleLinkedNode(void* data, 
                                      void* (*copyConstructor)(void*)) {
  SingleLinkedNode* node = (SingleLinkedNode*) malloc(sizeof(SingleLinkedNode));
  return initSingleLinkedNode(node, data, copyConstructor);
}

SingleLinkedNode* initSingleLinkedNode(SingleLinkedNode* node, void* data, 
                                       void* (*copyConstructor)(void*)) {
  node->data = copyConstructor(data);
  node->next = NULL;
  return node;
}

void destroySingleLinkedNode(SingleLinkedNode** nodePtr, 
                             void (*destructor)(void**)) {
  destructor(&(*nodePtr)->data);
  free(*nodePtr);
  *nodePtr = NULL;
}

void LinkedList_append(LinkedList* list, void* data) {
  SingleLinkedNode** appendLocation;
  if (list->firstNode != NULL) {
    appendLocation = &_LinkedList_lastNode(list)->next;
  } else {
    appendLocation = &list->firstNode;
  }

  *appendLocation = newSingleLinkedNode(data, list->_copyConstructor);
}

void LinkedList_clear(LinkedList* list) {
  SingleLinkedNode* nextNode = list->firstNode;
  while (nextNode != NULL) {
    SingleLinkedNode* tmp = nextNode->next;
    destroySingleLinkedNode(&nextNode, list->_destructor);
    nextNode = tmp;
  }

  list->firstNode = NULL;
}

void* LinkedList_first(const LinkedList* list) {
  return list->firstNode->data;
}

void LinkedList_forEach(const LinkedList* list, void (*action)(void*)) {
  SingleLinkedNode* nextNode = list->firstNode;
  while (nextNode != NULL) {
    action(nextNode->data);
    nextNode = nextNode->next;
  }
}

void* LinkedList_last(const LinkedList* list) {
  return _LinkedList_lastNode(list)->data;
}

void LinkedList_prepend(LinkedList* list, void* data) {
  SingleLinkedNode* oldFirst = list->firstNode;
  list->firstNode = newSingleLinkedNode(data, list->_copyConstructor);
  if (oldFirst != NULL) {
    list->firstNode->next = oldFirst;
  }
}

void LinkedList_removeFirst(LinkedList* list) {
  SingleLinkedNode* oldFirst = list->firstNode;
  list->firstNode = oldFirst->next;
  destroySingleLinkedNode(&oldFirst, list->_destructor);
}

void LinkedList_removeLast(LinkedList* list) {
  SingleLinkedNode* lastNode = _LinkedList_lastNode(list);
  _LinkedList_nullifyLast(list, lastNode);
  destroySingleLinkedNode(&lastNode, list->_destructor);
}

SingleLinkedNode* _LinkedList_lastNode(const LinkedList* list) {
  SingleLinkedNode* nextNode = list->firstNode;
  while (nextNode->next != NULL) {
    nextNode = nextNode->next;
  }

  return nextNode; 
}

void _LinkedList_nullifyLast(LinkedList* list, SingleLinkedNode* last) {
  if (list->firstNode != last) {
    SingleLinkedNode* nextToLast = list->firstNode;
    while (nextToLast->next != last) {
      nextToLast = nextToLast->next;
    }
    nextToLast->next = NULL;
  } else {
    list->firstNode = NULL;
  }
}