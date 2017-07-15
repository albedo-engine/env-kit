add_subdirectory(data)

list(APPEND ALBEDO_PREFILTER_SOURCES main.cpp)

# Adds binary executable
add_executable(albedo-prefilter ${ALBEDO_PREFILTER_SOURCES})
target_link_libraries(albedo-prefilter albedo_envtools)
