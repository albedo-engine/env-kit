add_subdirectory(data)
add_subdirectory(math)

list(APPEND ALBEDO_SOURCES env-kit.cpp)

add_library(albedo_envtools SHARED ${ALBEDO_SOURCES})
