cmake_minimum_required(VERSION 2.8.12)
project(cPowers C)

set(CMAKE_C_FLAGS "-std=c99")

include_directories(headers/cPowers)
add_library(cPowers src/vector.c src/stringVector.c src/linkedList.c)

option(test "Build all tests." OFF)
if (test)
  set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")
  add_subdirectory(lib/gtest)
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

  add_executable(cPowers-tests test/file_streams_test.cpp)
  target_link_libraries(cPowers-tests gtest gtest_main cPowers)
  add_test(tests cPowers-test)
endif()