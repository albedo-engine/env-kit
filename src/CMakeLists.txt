add_subdirectory(data)
add_subdirectory(math)
add_subdirectory(processors)
add_subdirectory(shaders)

add_library(albedo_envtools SHARED ${ALBEDO_SOURCES})
IF (_ALBEDO_TOOLS_MODE EQUAL 0)
    target_link_libraries(albedo_envtools GLEW glfw)
ELSEIF(_ALBEDO_TOOLS_MODE EQUAL 1)
    target_link_libraries(albedo_envtools GLEW glfw)
ENDIF()

