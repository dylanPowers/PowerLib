#include "linkedList.h"

#include "stdlib.h"
#include "string.h"

void _LinkedList_generateLastNode(LinkedList* list);

LinkedList* initLinkedList(LinkedList* list, size_t typeSize,
                           void* (*copyInitializer)(void*, const void*, SystemErr*),
                           void (*deInitializer)(void*)) {
  list->firstNode = NULL;
  list->lastNode = NULL;
  list->_copyInitializer = copyInitializer;
  list->_deInitializer = deInitializer;
  list->_typeSize = typeSize;
  return list;
}

LinkedList* initLinkedListCp(LinkedList* list, const LinkedList* copy, SystemErr* se) {
  initLinkedList(list, copy->_typeSize, copy->_copyInitializer,
                 copy->_deInitializer);
  SingleLinkedNode* nextNode = copy->firstNode;
  while (nextNode != NULL) {
    LinkedList_append(list, nextNode->data, se);
    nextNode = nextNode->next;
  }

  return list;
}


void deinitLinkedList(LinkedList* list) {
  LinkedList_clear(list);
}


SingleLinkedNode* initSingleLinkedNode(SingleLinkedNode* node, const void* data, 
                                       size_t typeSize,
                                       void* (*copyInitializer)(void*, const void*, SystemErr*),
                                       SystemErr* se) {
  initSingleLinkedNode_empty(node, typeSize, se);
  if (!*se) {
    copyInitializer(node->data, data, se);
  }

  return node;
}

SingleLinkedNode* initSingleLinkedNode_empty(SingleLinkedNode* node,
                                             size_t typeSize,
                                             SystemErr* se) {
  node->data = malloc(typeSize);
  if (node->data != NULL) {
    memset(node->data, 0, typeSize);
    node->next = NULL;
  } else { *se = S_E_NOMEMS; }
  return node;
}

void deinitSingleLinkedNode(SingleLinkedNode* node,
                            void (*deInitializer)(void*)) {
  deInitializer(node->data);
}

void LinkedList_append(LinkedList* list, const void* data, SystemErr* se) {
  _LinkedList_generateLastNode(list);
  initSingleLinkedNode(list->lastNode, data, list->_typeSize,
                       list->_copyInitializer, se);
}

void* LinkedList_appendEmpty(LinkedList* list, SystemErr* se) {
  _LinkedList_generateLastNode(list);
  initSingleLinkedNode_empty(list->lastNode, list->_typeSize, se);
  return list->lastNode->data;
}

void _LinkedList_generateLastNode(LinkedList* list) {
  list->lastNode = (SingleLinkedNode*) malloc(sizeof(SingleLinkedNode));

  if (list->firstNode != NULL) {
    list->lastNode->next = list->lastNode;
  } else {
    list->firstNode = list->lastNode;
  }
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
  list->lastNode = NULL;
}

void* LinkedList_first(const LinkedList* list) {
  return list->firstNode->data;
}


void* LinkedList_last(const LinkedList* list) {
  return list->lastNode->data;
}

void LinkedList_prepend(LinkedList* list, const void* data, SystemErr* se) {
  SingleLinkedNode* oldFirst = list->firstNode;
  list->firstNode = malloc(sizeof(SingleLinkedNode));
  initSingleLinkedNode(list->firstNode, data, list->_typeSize,
                       list->_copyInitializer, se);
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
  SingleLinkedNode* lastNode = list->lastNode;
  if (list->firstNode != lastNode) {
    SingleLinkedNode* nextToLast = list->firstNode;
    while (nextToLast->next != lastNode) {
      nextToLast = nextToLast->next;
    }
    nextToLast->next = NULL;
    list->lastNode = nextToLast;
  } else {
    list->firstNode = NULL;
    list->lastNode = NULL;
  }

  deinitSingleLinkedNode(lastNode, list->_deInitializer);
  free(lastNode);
}
