cmake_minimum_required(VERSION 3.20)

project(FluidSim)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

add_subdirectory(external/glfw)

add_library(glad
    external/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    external/glad/include
)

add_executable(FluidSim
    src/main.cpp
)

target_include_directories(FluidSim PRIVATE
    external/glm
)

target_link_libraries(FluidSim
    glfw
    glad
    OpenGL::GL
)