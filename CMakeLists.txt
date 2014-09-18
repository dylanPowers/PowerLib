cmake_minimum_required(VERSION 2.8.12)
project(cPowers C)

set(CMAKE_C_FLAGS "-std=c99")

include_directories(headers/cPowers)
add_library(cPowers src/vector.c src/stringVector.c src/linkedList.c)