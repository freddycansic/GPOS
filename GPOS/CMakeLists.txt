cmake_minimum_required(VERSION 3.8)

add_executable(${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/libraries/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/libraries/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/libraries/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/libraries/imgui/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/libraries/imgui/imgui_impl_opengl3.cpp
    ${CMAKE_SOURCE_DIR}/libraries/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/libraries/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/libraries/stb/stb_image.cpp
    ${CMAKE_SOURCE_DIR}/libraries/json/include/json/json.hpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/Application.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/ApplicationLauncher.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/Main.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/Debug.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/Stats.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/Util.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/Window.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/input/Button.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/input/Files.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/input/Input.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/input/Key.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/input/Keybind.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/input/MouseButton.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/ObjectRenderer.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/Renderer.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/gui/GUI.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/gui/Gizmo.cpp
    
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/objects/Line.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/objects/Cube.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/objects/Material.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/objects/Mesh.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/objects/Model.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/objects/Object.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/objects/Vertex.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/opengl/IndexBuffer.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/opengl/Shader.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/opengl/Texture.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/opengl/UniformBuffer.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/opengl/VertexArray.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/opengl/VertexBuffer.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/rendering/opengl/VertexBufferLayout.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/viewport/Camera.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/engine/viewport/Scene.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/maths/Maths.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/maths/Transform.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/maths/Vec2.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/maths/Vec3.cpp
    ${CMAKE_SOURCE_DIR}/GPOS/src/maths/Vec4.cpp
)

target_link_libraries(${PROJECT_NAME}
    glfw
    libglew_static
    nativefiledialog
    assimp
)

if (UNIX)
    target_compile_options(${PROJECT_NAME} PUBLIC "-Wno-narrowing")
endif()

# https://stackoverflow.com/a/60170862/17465501
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/res ${CMAKE_CURRENT_BINARY_DIR}/res
)