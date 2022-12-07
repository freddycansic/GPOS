cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE OpenGL_sources *.cpp ../libraries/imgui/*.cpp ../libraries/stb/*.cpp)

add_executable(${PROJECT_NAME} ${OpenGL_sources})

target_link_libraries(${PROJECT_NAME}
    glfw
    libglew_static
    #assimp
)

# https://stackoverflow.com/a/60170862/17465501
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMENT "Copied resources folder from ${CMAKE_CURRENT_SOURCE_DIR} to ${CMAKE_CURRENT_BINARY_DIR}"
)
