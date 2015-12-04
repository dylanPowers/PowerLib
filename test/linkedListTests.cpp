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
  // Did it crash?
}

class LinkedListMethods : public ::testing::Test {
public:
  LinkedListMethods() {
    se = S_E_CLEAR;
    initLinkedList(&list, sizeof(int), NULL, NULL);
  }

  virtual ~LinkedListMethods() {
    EXPECT_EQ(se, S_E_CLEAR);
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
  EXPECT_EQ(llErr, LL_E_NOT_FOUND);
}
