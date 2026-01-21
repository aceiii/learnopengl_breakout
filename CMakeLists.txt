cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME learnopengl-breakout)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -fno-rtti")

set(SOURCE_FILES
        src/main.cpp
        src/game.cpp
        src/shader.cpp
        src/texture.cpp
        src/gl_check.cpp
        src/sprite_renderer.cpp
        src/resource_manager.cpp
        src/game_object.cpp
        src/game_level.cpp
        src/ball_object.cpp
        src/particle_generator.cpp
        src/post_process.cpp
        src/powerup.cpp
        src/mp3audio.cpp
        src/text_renderer.cpp
        src/minimp3.c
        external/glad/src/glad.c
)

if(APPLE)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(COCOA_LIBRARY Cocoa)
    set(LIBS ${LIBS} ${OPENGL_LIBRARY} ${COCOA_LIBRARY})
endif()

add_subdirectory(external/glm)
add_subdirectory(external/freetype)
add_subdirectory(external/glfw)

#find_library(GLEW_LIBRARY glew REQUIRED)
#set(LIBS ${LIBS} ${GLEW_LIBRARY})

#add_subdirectory(external/glew/build/cmake)
#
#set(SOIL_LIBRARY soil)
#
#set(LIBS ${LIBS} ${SOIL_LIBRARY})
#
#find_library(GLFW_LIBRARY glfw3 REQUIRED)
#set(LIBS ${LIBS} ${GLFW_LIBRARY})
#
##find_library(SOLOUD_LIBRARY soloud REQUIRED)
#add_subdirectory(external/soloud/contrib)
#set(LIBS ${LIBS} ${SOLOUD_LIBRARY})
#
#find_package(Freetype REQUIRED)
#include_directories(${FREETYPE_INCLUDE_DIRS})
#set(LIBS ${LIBS} ${FREETYPE_LIBRARIES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE external/glad/include)

target_link_libraries(${PROJECT_NAME} ${LIBS} glm freetype glfw)

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    add_custom_target(symlink-resources ALL COMMAND
        ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res)
endif()
