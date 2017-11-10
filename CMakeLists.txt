cmake_minimum_required(VERSION 3.5)
project(env_kit)

set(CMAKE_CXX_STANDARD 14)

set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR})

set(BIN_SOURCE_FILES prefilter/main.cpp)

# You can specify whether you want to build the library
# with dependencies or not. In order to make the code
# work according to chosen dependencies, we have to set a
# definition for -D_ALBEDO_TOOLS_MODE.
set(ALBEDO_COMPILE_MODE ${_ALBEDO_TOOLS_MODE})
IF (NOT DEFINED ALBEDO_COMPILE_MODE)
    set(ALBEDO_COMPILE_MODE ALL)
ENDIF()

IF (ALBEDO_COMPILE_MODE STREQUAL ALL)
    set(ALBEDO_TOOLS_MODE 0)
ELSEIF (ALBEDO_COMPILE_MODE STREQUAL GPU_ONLY)
    set(ALBEDO_TOOLS_MODE 1)
ELSEIF (ALBEDO_COMPILE_MODE STREQUAL TBB_ONLY)
    set(ALBEDO_TOOLS_MODE 2)
ELSE ()
    set(ALBEDO_TOOLS_MODE 3)
ENDIF()
add_definitions(-DALBEDO_TOOLS_MODE=${ALBEDO_TOOLS_MODE})
add_definitions(-DALBEDO_TBB_GPU_MODE=0)
add_definitions(-DALBEDO_GPU_ONLY_MODE=1)
add_definitions(-DALBEDO_TBB_ONLY_MODE=2)

include_directories(
        ${CMAKE_SOURCE_DIR}/include
)

add_subdirectory(src)
add_subdirectory(prefilter)

