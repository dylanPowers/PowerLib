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

class StringMethods : public InitializationOfAString {
public:
  StringMethods() : InitializationOfAString() {
    SystemErr se = S_E_CLEAR;
    initString(&str, "", &se);
    conditionallyRaiseErr(se);
  }

  virtual ~StringMethods() {}
};

TEST_F(StringMethods, StringToiBase8Works) {
  SystemErr se = S_E_CLEAR;
  Vector_catPrimitive(&str, "10", 2, &se);
  int val = String_toi(&str, 8);
  EXPECT_EQ(8, val);
}
