#include "gtest/gtest.h"

extern "C" {
  #include "stringVector.h"
}

class InitializationOfAString : public ::testing::Test {
  public:
  InitializationOfAString() {

  }

  virtual ~InitializationOfAString() {
    deinitString(&str);
  }

  String str;
};

TEST_F(InitializationOfAString, SuccessfullyExecutes) {
  initString(&str, "");
  SUCCEED();
}

TEST_F(InitializationOfAString, HasCorrentLength) {
  initString(&str, "...");
  EXPECT_EQ(str.length, 3);
}