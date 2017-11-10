add_subdirectory(data)
add_subdirectory(processors)
add_subdirectory(shaders)

set(TBB_DIR $ENV{TBB_DIR})


add_library(albedo_envtools SHARED ${ALBEDO_SOURCES})

# This conditions allow to bind only the needed libraries.
# By default, it tries to bind everything.
# It is possible to only use one CPU thread, without library,
# to compute the maps, but it makes the whole pipeline really slow.

message(STATUS "Selected compilation mode: " ${ALBEDO_COMPILE_MODE})

IF (ALBEDO_COMPILE_MODE STREQUAL ALL)
    find_package(TBB)
    find_package(GLEW)
    find_package(glfw3)
    message("Linking albedo tools with GLEW, glfw, and TBB")
    target_link_libraries(albedo_envtools GLEW glfw ${TBB_IMPORTED_TARGETS})
ELSEIF(ALBEDO_COMPILE_MODE STREQUAL TBB_ONLY)
    find_package(TBB)
    message("Linking albedo tools with TBB")
    target_link_libraries(albedo_envtools ${TBB_IMPORTED_TARGETS})
ELSEIF(ALBEDO_COMPILE_MODE STREQUAL GPU_ONLY)
    find_package(GLEW)
    find_package(glfw3)
    message("Linking albedo tools with GLEW, glfw")
    target_link_libraries(albedo_envtools GLEW glfw)
ENDIF()
