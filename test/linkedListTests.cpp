#include "gtest/gtest.h"

extern "C" {
  #include "linkedList.h"
}

class InitializationOfASingleLinkedNode : public ::testing::Test {
public:
  virtual ~InitializationOfASingleLinkedNode() {
    deinitSingleLinkedNode(&node, sizeof(int), NULL);
  }

  SystemErr se = S_E_CLEAR;
  SingleLinkedNode node = {};
};

TEST_F(InitializationOfASingleLinkedNode, HandlesNullCopyInitializer) {
  int item = 1;
  initSingleLinkedNode(&node, &item, sizeof(int), NULL, &se);
  SUCCEED();
}

class LinkedListMethods : public ::testing::Test {
public:
  LinkedListMethods() {
    se = S_E_CLEAR;
    initLinkedList(&list, sizeof(int), NULL, NULL);
  }

  virtual ~LinkedListMethods() {
    deinitLinkedList(&list);
  }

  SystemErr se = S_E_CLEAR;
  LinkedList list = {};
};

bool intTest(int* firstInt, int* itemInt) {
  return *firstInt == *itemInt;
}

TEST_F(LinkedListMethods, DoesntFind2) {
  int item = 1;
  LinkedList_append(&list, &item, &se);
  int item2 = 2;
  LLErr llErr = LL_E_CLEAR;
  LinkedList_find(&list, &item2, (bool (*)(void*, void*)) &intTest, &llErr);
  EXPECT_EQ(LL_E_NOT_FOUND, llErr);
}

TEST_F(LinkedListMethods, FindReturnsPointToValue) {
  int item = 1;
  LinkedList_append(&list, &item, &se);
  LLErr llErr = LL_E_CLEAR;
  int* itemPtr = (int*) LinkedList_find(&list, &item, (bool (*)(void*, void*)) &intTest, &llErr);
  EXPECT_EQ(1, *itemPtr);
}

TEST_F(LinkedListMethods, HasLength1WhenItemAdded) {
  int item = 1;
  LinkedList_append(&list, &item, &se);
  EXPECT_EQ(1, list.length);
}

TEST_F(LinkedListMethods, HasLength1WhenItemPrepended) {
  int item = 1;
  LinkedList_prepend(&list, &item, &se);
  EXPECT_EQ(1, list.length);
}

TEST_F(LinkedListMethods, LengthDecrementsWhenFirstRemoved) {
  int item = 1;
  LinkedList_append(&list, &item, &se);
  LinkedList_removeFirst(&list);
  EXPECT_EQ(0, list.length);
}

TEST_F(LinkedListMethods, LengthDecrementsWhenLastRemoved) {
  int item = 1;
  LinkedList_append(&list, &item, &se);
  LinkedList_removeLast(&list);
  EXPECT_EQ(0, list.length);
}

TEST_F(LinkedListMethods, FirstNodeNextPointsToLast) {
  int item = 1;
  LinkedList_append(&list, &item, &se);
  item = 2;
  LinkedList_append(&list, &item, &se);
  EXPECT_EQ(list.firstNode->next, list.lastNode);
}

TEST_F(LinkedListMethods, NodesNullWhenFirstRemovedOnLength1List) {
  int item = 1;
  LinkedList_append(&list, &item, &se);
  LinkedList_removeFirst(&list);
  EXPECT_EQ(NULL, list.firstNode);
  EXPECT_EQ(NULL, list.lastNode);
}

TEST_F(LinkedListMethods, ClearSetsLengthTo0) {
  int item = 1;
  LinkedList_append(&list, &item, &se);
  LinkedList_clear(&list);
  EXPECT_EQ(0, list.length);
}
