cmake_minimum_required(VERSION 3.0.0)
project(hw2_static)

add_subdirectory(hw2_lib)
add_subdirectory(hw2_lib_shared)

message("HW2_INCLUDE_DIRS = ${HW2_INCLUDE_DIRS}")
message("HW2_LIBRARIES = ${HW2_LIBRARIES}")

message("HW2_SHARED_DIRS = ${HW2_INCLUDE_DIRS}")
message("HW2_SHARED = ${HW2_LIBRARIES}")

add_executable(prepare_test prepare_test.c)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}" )

target_include_directories(prepare_test PUBLIC ${HW2_INCLUDE_DIRS})
target_link_libraries(prepare_test PRIVATE ${HW2_LIBRARIES})

add_executable(${PROJECT_NAME} stress_test.c)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )

target_include_directories(${PROJECT_NAME} PUBLIC ${HW2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${HW2_LIBRARIES})

add_executable(hw2_shared stress_test.c)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov -pthread" )

target_include_directories(hw2_shared PUBLIC ${HW2_SHARED_DIRS})
target_link_libraries(hw2_shared PRIVATE ${HW2_SHARED})

add_subdirectory(tests)
