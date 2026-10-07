cmake_minimum_required(VERSION 4.2)
project(opengl_demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Configure GLFW options before adding its subdirectory
# This prevents compiling GLFW documentation, tests, and examples
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Add GLFW from your local deps path
add_subdirectory(deps/glfw)

# 2. Configure GLAD as a static library
# This includes the auto-generated glad.c and exposes its include directory
add_library(glad STATIC deps/glad/src/glad.c)
target_include_directories(glad PUBLIC deps/glad/include)

# 3. Find system OpenGL
find_package(OpenGL REQUIRED)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE deps/stb)


# 4. Define your main executable project
add_executable(${PROJECT_NAME} src/main.cpp
        include/shader.hpp
        src/shader.cpp
        include/VBO.hpp
        src/VBO.cpp
        include/EBO.hpp
        src/EBO.cpp
        include/VAO.hpp
        src/VAO.cpp
        src/stb_image_impl.cpp
        include/texture.h
        src/texture.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -Wpedantic -Wextra)
target_include_directories(${PROJECT_NAME} PRIVATE include)

# 5. Link libraries and include headers for your main project
target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw OpenGL::GL stb_image)