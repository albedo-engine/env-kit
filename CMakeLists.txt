cmake_minimum_required(VERSION 3.5)
project(env_kit)

set(CMAKE_CXX_STANDARD 14)

set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR})

set(BIN_SOURCE_FILES main.cpp)

include_directories(
        ${CMAKE_SOURCE_DIR}/src
)

add_subdirectory(src)

# Adds binary executable
add_executable(env-kit ${BIN_SOURCE_FILES})
target_link_libraries(env-kit albedo_envtools)

