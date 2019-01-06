###############################################################################
#                                  VARIABLES                                  #
###############################################################################
set(ENVKIT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(ENVKIT_SRC
    src/glad.cpp
    src/opengl-processor.cpp
)

###############################################################################
#                                    TARGET                                   #
###############################################################################
add_library(envkit ${SHARED_OR_STATIC} ${ENVKIT_SRC})
target_include_directories(envkit PUBLIC ${ENVKIT_INCLUDE_DIR})
