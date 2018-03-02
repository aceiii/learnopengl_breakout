cmake_minimum_required(VERSION 3.9)

set(PROJECT_NAME learnopengl-breakout)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -fno-rtti")

set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/minimp3.c)

if(APPLE)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(COCOA_LIBRARY Cocoa)
    set(LIBS ${LIBS} ${OPENGL_LIBRARY} ${COCOA_LIBRARY})
endif()

find_library(GLEW_LIBRARY glew REQUIRED)
set(LIBS ${LIBS} ${GLEW_LIBRARY})

find_library(SOIL_LIBRARY soil REQUIRED)
set(LIBS ${LIBS} ${SOIL_LIBRARY})

find_library(GLFW_LIBRARY glfw3 REQUIRED)
set(LIBS ${LIBS} ${GLFW_LIBRARY})

find_library(SOLOUD_LIBRARY soloud REQUIRED)
set(LIBS ${LIBS} ${SOLOUD_LIBRARY})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
set(LIBS ${LIBS} ${FREETYPE_LIBRARIES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${LIBS})

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    add_custom_target(symlink-resources ALL COMMAND
        ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res)
endif()
