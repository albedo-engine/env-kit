add_executable(check main.cpp)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/includes)

TARGET_LINK_LIBRARIES(check ${GTEST_LIBRARIES} pthread albedo_envtools)
