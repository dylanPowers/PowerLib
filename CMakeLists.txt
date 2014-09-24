cmake_minimum_required(VERSION 2.8.12)
project(cPowers)

set(CMAKE_C_FLAGS "-Wall -std=c99")

include_directories(headers/cPowers)
add_library(cPowers src/vector.c src/stringVector.c src/linkedList.c)

option(test_cPowers "Build all tests." OFF)

if (test_cPowers)
  set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

  add_subdirectory(lib/gtest)
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

  add_executable(cPowers-tests test/vectorTests.cpp test/stringTests.cpp)
  target_link_libraries(cPowers-tests cPowers gtest gtest_main)

  enable_testing()
  add_test(tests cPowers-tests)
endif()