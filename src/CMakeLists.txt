add_subdirectory(data)
add_subdirectory(math)
add_subdirectory(processors)

add_library(albedo_envtools SHARED ${ALBEDO_SOURCES})
