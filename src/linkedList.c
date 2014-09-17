#include "linkedList.h"

#include "stdlib.h"
#include "string.h"

SingleLinkedNode* _LinkedList_lastNode(const LinkedList*);
void _LinkedList_nullifyLast(LinkedList*, SingleLinkedNode*);

struct LinkedList {
  SingleLinkedNode* firstNode;
  int length;

  void* (*_copyInitializer)(void*);
  void (*_deInitializer)(void*);
  int _typeSize;
};

struct SingleLinkedNode {
  void* data;
  SingleLinkedNode* next;
};

LinkedList* newLinkedList(int typeSize, void* (*_copyInitializer)(void*), 
                          void (*deInitializer)(void*)) {
  LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
  return initLinkedList(list, typeSize, _copyInitializer, deInitializer);
}

LinkedList* initLinkedList(LinkedList* list, int typeSize, 
                           void* (*copyInitializer)(void*), 
                           void (*deInitializer)(void*)) {
  list->firstNode = NULL;
  list->length = 0;
  list->_copyInitializer = copyInitializer;
  list->_deInitializer = deInitializer;
  return list;
}

void destroyLinkedList(LinkedList** listPtr) {
  LinkedList_clear(*listPtr);
  free(*listPtr);
  *listPtr = NULL;
}

SingleLinkedNode* newSingleLinkedNode(void* data, int typeSize,
                                      void* (*copyInitializer)(void*)) {
  SingleLinkedNode* node = (SingleLinkedNode*) malloc(sizeof(SingleLinkedNode));
  return initSingleLinkedNode(node, data, typeSize, copyInitializer);
}

SingleLinkedNode* initSingleLinkedNode(SingleLinkedNode* node, void* data, 
                                       int typeSize, 
                                       void* (*copyInitializer)(void*)) {
  node->data = malloc(typeSize);
  copyInitializer(node->data);
  node->next = NULL;
  return node;
}

void destroySingleLinkedNode(SingleLinkedNode** nodePtr, 
                             void (*deInitializer)(void*)) {
  deInitializer((*nodePtr)->data);
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

  *appendLocation = newSingleLinkedNode(data, list->_typeSize, 
                                        list->_copyInitializer);
}

void LinkedList_clear(LinkedList* list) {
  SingleLinkedNode* nextNode = list->firstNode;
  while (nextNode != NULL) {
    SingleLinkedNode* tmp = nextNode->next;
    destroySingleLinkedNode(&nextNode, list->_deInitializer);
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
  list->firstNode = newSingleLinkedNode(data, list->_typeSize, 
                                        list->_copyInitializer);
  if (oldFirst != NULL) {
    list->firstNode->next = oldFirst;
  }
}

void LinkedList_removeFirst(LinkedList* list) {
  SingleLinkedNode* oldFirst = list->firstNode;
  list->firstNode = oldFirst->next;
  destroySingleLinkedNode(&oldFirst, list->_deInitializer);
}

void LinkedList_removeLast(LinkedList* list) {
  SingleLinkedNode* lastNode = _LinkedList_lastNode(list);
  _LinkedList_nullifyLast(list, lastNode);
  destroySingleLinkedNode(&lastNode, list->_deInitializer);
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