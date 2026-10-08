cmake_minimum_required(VERSION 3.24)
project(opengl_demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/glfw)

# 2. GLAD
add_library(glad STATIC deps/glad/src/glad.c)
target_include_directories(glad PUBLIC deps/glad/include)

# 3. System OpenGL
find_package(OpenGL REQUIRED)

# 4. GLM (Header-only)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE deps)
add_library(glm::glm ALIAS glm)

# 5. stb_image (Compiled once)
add_library(stb_image STATIC src/stb_image_impl.cpp)
target_include_directories(stb_image PUBLIC deps/stb)

# 6. Main Executable
add_executable(${PROJECT_NAME}
        src/main.cpp
        include/shader.hpp
        src/shader.cpp
        include/VBO.hpp
        src/VBO.cpp
        include/EBO.hpp
        src/EBO.cpp
        include/VAO.hpp
        src/VAO.cpp
        include/texture.h
        src/texture.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        glad
        glfw
        OpenGL::GL
        stb_image
        glm::glm
)

# 7. Copy runtime dependencies
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shader $<TARGET_FILE_DIR:${PROJECT_NAME}>/shader
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)