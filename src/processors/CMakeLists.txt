add_subdirectory(cpu)

IF ((ALBEDO_COMPILE_MODE STREQUAL GPU_ONLY)
        OR (ALBEDO_COMPILE_MODE STREQUAL ALL))
    set(ALBEDO_SOURCES ${ALBEDO_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/abstract-processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpu-processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gpu-processor.cpp
        PARENT_SCOPE)
ELSE()
    set(ALBEDO_SOURCES ${ALBEDO_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/abstract-processor.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/cpu-processor.cpp
            PARENT_SCOPE)
ENDIF()