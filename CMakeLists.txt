cmake_minimum_required(VERSION 3.8)

project(OpenGL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")
endif()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
	add_definitions(-DDEBUG) # preprocessor macro
else()
	if (MSVC)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") # if running on release mode remove the cmd prompt on windows
	endif()
endif()

add_subdirectory(OpenGL)
add_subdirectory(libraries/glfw)
add_subdirectory(libraries/glew)
#add_subdirectory(libraries/assimp)

#set(BUILD_SHARED_LIBS OFF) # make assimp compile to a static library

target_include_directories(${PROJECT_NAME} PRIVATE {CMAKE_CURRENT_SOURCE_DIR}/libraries/glfw/include)
target_include_directories(${PROJECT_NAME} PRIVATE {CMAKE_CURRENT_SOURCE_DIR}/libraries/glew/include)
target_include_directories(${PROJECT_NAME} PRIVATE OpenGL/src)
target_include_directories(${PROJECT_NAME} PRIVATE libraries)
target_include_directories(${PROJECT_NAME} PRIVATE libraries/stb)