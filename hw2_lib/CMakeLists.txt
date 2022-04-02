cmake_minimum_required(VERSION 3.0.0)
project(hw2_lib)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-fprofile-arcs -ftest-coverage -fPIC -O0 -Wall -Werror -Wpedantic")

file(GLOB SOURCES *.c)
file(GLOB INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME} ${SOURCES} inout.c inout.h)

set(HW2_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)
set(HW2_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
