#include "gtest/gtest.h"

extern "C" {
  #include "stringVector.h"
}

TEST(StringsTest, StringInitializes) {
  String str;
  initString(&str, "");
  SUCCEED();
}