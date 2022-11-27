cmake_minimum_required(VERSION 3.0)

project(OpenGL3D)

set(srcDir OpenGL/src/)
set(libsDir OpenGL/libs/)

file(GLOB_RECURSE OpenGL3D_sources *.cpp *.h)

add_executable(OpenGL3D_sources ${sources})

include_directories(${srcDir} ${libsDir})

link_directories(${libsDir}GLFW/lib ${libsDir}GLEW/lib)

add_library(glfw3 STATIC OpenGL/libs/GLFW/lib/glfw3.lib)
add_library(glew STATIC OpenGL/libs/GLEW/lib/glew32s.lib)

target_link_libraries(glfw3 glew opengl32.lib User32.lib Gdi32.lib Shell32.lib)