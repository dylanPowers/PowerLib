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
  Vector v;
};

TEST_F(InitializationOfAVector, SuccessfullyExecutes) {
  initVector(&v, 1, NULL, NULL);
  SUCCEED();
}

void* dummyCopyInitializer(void* a, const void* b) {
  return a;
}

TEST_F(InitializationOfAVector, HasASetCopyInitializer) {
  initVector(&v, 1, &dummyCopyInitializer, NULL);
  EXPECT_EQ(&dummyCopyInitializer, v._copyInitializer);
}

TEST_F(InitializationOfAVector, HasCorrectLength) {
  initVector(&v, sizeof(int), NULL, NULL);
  EXPECT_EQ(v.length, 0);
}

class VectorMethods : public InitializationOfAVector {
  public:

  VectorMethods() {
    initVector(&v, sizeof(int), NULL, NULL);
  };
};

TEST_F(VectorMethods, ReverseCompletes) {
  Vector reversed;
  initVector(&reversed, sizeof(int), NULL, NULL);
  int nums[3] = { 1, 2, 3 };
  Vector_catPrimitive(&v, nums, 3);
  Vector_reverse(&v, &reversed);
  EXPECT_EQ(*(int*) reversed.arr, 3);
  EXPECT_EQ(*((int*) reversed.arr + 1), 2);
  EXPECT_EQ(*((int*) reversed.arr + 2), 1);

  deinitVector(&reversed);
}

