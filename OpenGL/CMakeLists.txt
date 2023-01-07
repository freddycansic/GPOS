cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE OpenGL_sources *.cpp ../libraries/imgui/*.cpp ../libraries/stb/*.cpp)

add_executable(${PROJECT_NAME} ${OpenGL_sources} "src/engine/input/Key.cpp" "src/engine/input/Buttons.h" "src/engine/input/Button.h" "src/engine/input/Button.cpp" "src/engine/input/MouseButton.h" "src/engine/input/MouseButton.cpp" "src/engine/rendering/objects/Model.h" "src/engine/rendering/objects/Model.cpp")

add_library(assimp STATIC IMPORTED)
set_property(TARGET assimp PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libraries/assimp/lib/assimp-vc143-mtd.lib)

add_library(zlib STATIC IMPORTED )
set_property(TARGET zlib PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libraries/zlib/lib/zlibstaticd.lib)

target_link_libraries(${PROJECT_NAME}
    glfw
    libglew_static
    nativefiledialog
    assimp
    zlib
)

# https://stackoverflow.com/a/60170862/17465501
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMENT "Copied resources folder from ${CMAKE_CURRENT_SOURCE_DIR} to ${CMAKE_CURRENT_BINARY_DIR}"
)