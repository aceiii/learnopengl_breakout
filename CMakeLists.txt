cmake_minimum_required(VERSION 3.9)

set(PROJECT_NAME learnopengl-breakout)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/main.cpp)

if(APPLE)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(COCOA_LIBRARY Cocoa)
    set(LIBS ${LIBS} ${OPENGL_LIBRARY} ${COCOA_LIBRARY})
endif()

find_library(GLEW_LIBRARY glew REQUIRED)
set(LIBS ${LIBS} ${GLEW_LIBRARY})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${LIBS})

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    add_custom_target(symlink-resources ALL COMMAND
        ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res)
endif()
