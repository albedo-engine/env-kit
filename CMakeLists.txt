cmake_minimum_required(VERSION 3.5)
project(env_kit)

set(CMAKE_CXX_STANDARD 14)

set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR})

set(BIN_SOURCE_FILES prefilter/main.cpp)

include_directories(
        ${CMAKE_SOURCE_DIR}/include
)

add_subdirectory(src)
add_subdirectory(prefilter)
add_subdirectory(tests)


