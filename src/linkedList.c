#include "linkedList.h"

#ifndef __BCC__

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
    if (copyInitializer) {
      copyInitializer(node->data, data, se);
    } else {
      memcpy(node->data, data, typeSize);
    }
  }

  return node;
}

SingleLinkedNode* initSingleLinkedNode_empty(SingleLinkedNode* node,
                                             size_t typeSize,
                                             SystemErr* se) {
  node->next = NULL;
  node->data = malloc(typeSize);
  if (node->data != NULL) {
    memset(node->data, 0, typeSize);
  } else { *se = S_E_NOMEMS; }
  return node;
}

void deinitSingleLinkedNode(SingleLinkedNode* node, size_t typeSize,
                            void (*deInitializer)(void*)) {
  if (deInitializer) {
    deInitializer(node->data);
  } else {
    memset(node->data, 0, typeSize);
  }
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
  SingleLinkedNode* lastNode = (SingleLinkedNode*) malloc(sizeof(SingleLinkedNode));
  list->length++;

  if (list->firstNode == NULL) {
    list->firstNode = lastNode;
  } else {
    list->lastNode->next = lastNode;
  }

  list->lastNode = lastNode;
}


void LinkedList_clear(LinkedList* list) {
  SingleLinkedNode* nextNode = list->firstNode;
  while (nextNode != NULL) {
    SingleLinkedNode* tmp = nextNode->next;
    deinitSingleLinkedNode(nextNode, list->_typeSize, list->_deInitializer);
    free(nextNode);
    nextNode = tmp;
  }

  list->length = 0;
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
  list->length++;
}


void LinkedList_removeFirst(LinkedList* list) {
  SingleLinkedNode* oldFirst = list->firstNode;
  if (list->firstNode->next) {
    list->firstNode = oldFirst->next;
  } else {
    list->firstNode = NULL;
    list->lastNode = NULL;
  }
  deinitSingleLinkedNode(oldFirst, list->_typeSize, list->_deInitializer);
  free(oldFirst);
  list->length--;
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

  deinitSingleLinkedNode(lastNode, list->_typeSize, list->_deInitializer);
  free(lastNode);
  list->length--;
}

/**
 * Finds an item in the linked list with the given comparison function.
 * @errors LL_E_NOT_FOUND
 */
void* LinkedList_find(LinkedList* list, void* dataToFind,
                     bool (*cmp)(void* dataToFind, void* itemData), LLErr* le) {
  SingleLinkedNode *node = list->firstNode;
  while (node != NULL) {
    if (cmp(dataToFind, node->data)) {
      return node->data;
    }

    node = node->next;
  }

  *le = LL_E_NOT_FOUND;

  return NULL;
}

#endif
