cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE OpenGL_sources *.cpp ../libraries/imgui/*.cpp ../libraries/stb/*.cpp)

add_executable(OpenGL ${OpenGL_sources})

target_link_libraries(OpenGL
    glfw
    libglew_static
    #assimp
)