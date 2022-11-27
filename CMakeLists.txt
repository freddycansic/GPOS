cmake_minimum_required(VERSION 3.8)

project(OpenGL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(OpenGL)
add_subdirectory(libraries/glfw)
add_subdirectory(libraries/glew)

target_include_directories(OpenGL PRIVATE {CMAKE_CURRENT_SOURCE_DIR}/libraries/glfw/include)
target_include_directories(OpenGL PRIVATE {CMAKE_CURRENT_SOURCE_DIR}/libraries/glew/include)
target_include_directories(OpenGL PRIVATE OpenGL/src)
target_include_directories(OpenGL PRIVATE libraries)
target_include_directories(OpenGL PRIVATE libraries/stb)