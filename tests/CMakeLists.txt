cmake_minimum_required(VERSION 3.14)
project(test_static)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fprofile-arcs -ftest-coverage -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )

file(GLOB SOURCES *.cpp)

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${HW2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${HW2_LIBRARIES} GTest::GTest gtest_main)

add_executable(test_shared ${SOURCES})
target_include_directories(test_shared PUBLIC ${HW2_SHARED_DIRS})
target_link_libraries(test_shared ${HW2_SHARED} GTest::GTest gtest_main)