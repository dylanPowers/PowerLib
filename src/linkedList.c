#include "linkedList.h"

#include "stdlib.h"
#include "string.h"

SingleLinkedNode* _LinkedList_lastNode(const LinkedList*);
void _LinkedList_nullifyLast(LinkedList*, SingleLinkedNode*);

LinkedList* initLinkedList(LinkedList* list, size_t typeSize,
                           void* (*copyInitializer)(void*, const void*, void*),
                           void (*deInitializer)(void*)) {
  list->firstNode = NULL;
  list->_copyInitializer = copyInitializer;
  list->_deInitializer = deInitializer;
  list->_typeSize = typeSize;
  return list;
}

LinkedList* initLinkedListCp(LinkedList* list, const LinkedList* copy) {
  initLinkedList(list, copy->_typeSize, copy->_copyInitializer,
                 copy->_deInitializer);
  SingleLinkedNode* nextNode = copy->firstNode;
  while (nextNode != NULL) {
    LinkedList_append(list, nextNode->data);
    nextNode = nextNode->next;
  }

  return list;
}


void deinitLinkedList(LinkedList* list) {
  LinkedList_clear(list);
}


SingleLinkedNode* initSingleLinkedNode(SingleLinkedNode* node, const void* data, 
                                       size_t typeSize,
                                       void* (*copyInitializer)(void*, const void*, void*)) {
  node->data = malloc(typeSize);
  memset(node->data, 0, typeSize);
  int err = 0;
  copyInitializer(node->data, data, &err);
  node->next = NULL;
  return node;
}


void deinitSingleLinkedNode(SingleLinkedNode* node,
                            void (*deInitializer)(void*)) {
  deInitializer(node->data);
}

void LinkedList_append(LinkedList* list, const void* data) {
  SingleLinkedNode** appendLocation;
  if (list->firstNode != NULL) {
    appendLocation = &_LinkedList_lastNode(list)->next;
  } else {
    appendLocation = &list->firstNode;
  }

  SingleLinkedNode* node = (SingleLinkedNode*) malloc(sizeof(SingleLinkedNode));
  *appendLocation = initSingleLinkedNode(node, data, list->_typeSize,
                                         list->_copyInitializer);
}

void LinkedList_clear(LinkedList* list) {
  SingleLinkedNode* nextNode = list->firstNode;
  while (nextNode != NULL) {
    SingleLinkedNode* tmp = nextNode->next;
    deinitSingleLinkedNode(nextNode, list->_deInitializer);
    free(nextNode);
    nextNode = tmp;
  }

  list->firstNode = NULL;
}

void* LinkedList_first(const LinkedList* list) {
  return list->firstNode->data;
}


void* LinkedList_last(const LinkedList* list) {
  return _LinkedList_lastNode(list)->data;
}

void LinkedList_prepend(LinkedList* list, const void* data) {
  SingleLinkedNode* oldFirst = list->firstNode;
  list->firstNode = malloc(sizeof(SingleLinkedNode));
  initSingleLinkedNode(list->firstNode, data, list->_typeSize,
                       list->_copyInitializer);
  if (oldFirst != NULL) {
    list->firstNode->next = oldFirst;
  }
}

void LinkedList_removeFirst(LinkedList* list) {
  SingleLinkedNode* oldFirst = list->firstNode;
  list->firstNode = oldFirst->next;
  deinitSingleLinkedNode(oldFirst, list->_deInitializer);
  free(oldFirst);
}

void LinkedList_removeLast(LinkedList* list) {
  SingleLinkedNode* lastNode = _LinkedList_lastNode(list);
  _LinkedList_nullifyLast(list, lastNode);
  deinitSingleLinkedNode(lastNode, list->_deInitializer);
  free(lastNode);
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
