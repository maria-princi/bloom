cmake_minimum_required(VERSION 3.14)
project(bloom)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(bloom glad.c main.cpp glfw-3.3/Init.cpp glfw-3.3/Init.h shader.cpp stb_image.h stb_image.cpp)

add_subdirectory(../bloom/glfw-3.3)

find_package(OpenGL REQUIRED)

target_link_libraries(bloom glfw)