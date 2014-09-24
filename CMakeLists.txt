cmake_minimum_required(VERSION 2.8.12)
project(cPowers)

set(CMAKE_C_FLAGS "-Wall -std=c99")
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

include_directories(headers/cPowers)
add_library(cPowers src/vector.c src/stringVector.c src/linkedList.c)

option(test "Build all tests." OFF)
set(test test)
if (test)
  add_subdirectory(lib/gtest)

  enable_testing()
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

  add_executable(cPowers-tests test/stringTests.cpp)
  target_link_libraries(cPowers-tests cPowers gtest gtest_main)

  add_test(tests cPowers-tests)
endif()