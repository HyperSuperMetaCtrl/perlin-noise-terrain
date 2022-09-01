cmake_minimum_required(VERSION 3.17)
project(cg_projekt)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        include
        libs/include)
link_directories(libs/lib)
add_executable(cg_projekt
        src/main.cpp
        src/Terrain.cpp
        src/Camera.cpp
        src/ShaderProgram.cpp
        src/lodepng.cpp
        src/Skybox.cpp
        src/stbi_image.cpp)
# set the path to the library folder
# link the libraries to the executable
find_library(GLEW_LIB NAMES GLEW PATHS ${CMAKE_SOURCE_DIR}/libs/lib)
find_library(GLFW_LIB NAMES glfw PATHS ${CMAKE_SOURCE_DIR}/libs/lib)
find_library(NOISE_LIB NAMES noise PATHS ${CMAKE_SOURCE_DIR}/libs/lib
        NO_DEFAULT_PATHS)

target_link_libraries (cg_projekt GL glfw noise GLEW)
target_compile_options(cg_projekt PRIVATE -O3)
