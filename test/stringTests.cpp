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

  String str = {};
};

TEST_F(InitializationOfAString, SuccessfullyExecutes) {
  SystemErr eIgnore;
  initString(&str, "", &eIgnore);
  SUCCEED();
}

TEST_F(InitializationOfAString, HasCorrentLength) {
  SystemErr eIgnore;
  initString(&str, "...", &eIgnore);
  EXPECT_EQ(str.length, 3);
}
