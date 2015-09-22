#include "gtest/gtest.h"

extern "C" {
  #include "vector.h"
}

class InitializationOfAVector : public ::testing::Test {
public:
  // You can remove any or all of the following functions if its body
  // is empty.

  InitializationOfAVector() {
    // You can do set-up work for each test here.
  }

  virtual ~InitializationOfAVector() {
    deinitVector(&v);
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Project1.
  Vector v = {};
};

TEST_F(InitializationOfAVector, SuccessfullyExecutes) {
  SystemErr eIgnore = S_E_CLEAR;
  initVector(&v, 1, NULL, NULL, &eIgnore);
  SUCCEED();
}

void* dummyCopyInitializer(void* a, const void* b, SystemErr* err) {
  return a;
}

TEST_F(InitializationOfAVector, HasASetCopyInitializer) {
  SystemErr eIgnore = S_E_CLEAR;
  initVector(&v, 1, &dummyCopyInitializer, NULL, &eIgnore);
  EXPECT_EQ(&dummyCopyInitializer, v._copyInitializer);
}

TEST_F(InitializationOfAVector, HasCorrectLength) {
  SystemErr eIgnore = S_E_CLEAR;
  initVector(&v, sizeof(int), NULL, NULL, &eIgnore);
  EXPECT_EQ(v.length, 0);
}

class VectorMethods : public InitializationOfAVector {
public:

  VectorMethods() {
    SystemErr eIgnore = S_E_CLEAR;
    initVector(&v, sizeof(int), NULL, NULL, &eIgnore);
  };
};

TEST_F(VectorMethods, ReverseCompletes) {
  Vector reversed = {};
  SystemErr eIgnore = S_E_CLEAR;
  initVector(&reversed, sizeof(int), NULL, NULL, &eIgnore);
  int nums[3] = { 1, 2, 3 };
  Vector_catPrimitive(&v, nums, 3, &eIgnore);
  Vector_reverse(&v, &reversed, &eIgnore);
  EXPECT_EQ(*(int*) reversed.arr, 3);
  EXPECT_EQ(*((int*) reversed.arr + 1), 2);
  EXPECT_EQ(*((int*) reversed.arr + 2), 1);

  deinitVector(&reversed);
}

