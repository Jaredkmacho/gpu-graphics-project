cmake_minimum_required(VERSION 3.10)

project(GPUGraphicsProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set vcpkg paths
set(VCPKG_ROOT "$ENV{VCPKG_ROOT}")
set(VCPKG_INSTALLED_DIR "${VCPKG_ROOT}/installed/x64-windows")

# Debug information
message(STATUS "VCPKG_ROOT: ${VCPKG_ROOT}")
message(STATUS "VCPKG_INSTALLED_DIR: ${VCPKG_INSTALLED_DIR}")

# Set include and library directories
set(GLEW_INCLUDE_DIRS "${VCPKG_INSTALLED_DIR}/include")
set(GLFW_INCLUDE_DIRS "${VCPKG_INSTALLED_DIR}/include")
set(GLUT_INCLUDE_DIRS "${VCPKG_INSTALLED_DIR}/include")

# Set library paths based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(GLEW_LIBRARIES "${VCPKG_INSTALLED_DIR}/debug/lib/glew32d.lib")
    set(GLFW_LIBRARIES "${VCPKG_INSTALLED_DIR}/debug/lib/glfw3dll.lib")
    set(GLUT_LIBRARIES "${VCPKG_INSTALLED_DIR}/debug/lib/freeglutd.lib")
else()
    set(GLEW_LIBRARIES "${VCPKG_INSTALLED_DIR}/lib/glew32.lib")
    set(GLFW_LIBRARIES "${VCPKG_INSTALLED_DIR}/lib/glfw3dll.lib")
    set(GLUT_LIBRARIES "${VCPKG_INSTALLED_DIR}/lib/freeglut.lib")
endif()

# Debug information for package finding
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

# Find required packages
find_package(GLEW REQUIRED)
message(STATUS "GLEW_INCLUDE_DIRS: ${GLEW_INCLUDE_DIRS}")
message(STATUS "GLEW_LIBRARIES: ${GLEW_LIBRARIES}")

find_package(glfw3 REQUIRED)
message(STATUS "GLFW_INCLUDE_DIRS: ${GLFW_INCLUDE_DIRS}")
message(STATUS "GLFW_LIBRARIES: ${GLFW_LIBRARIES}")

find_package(FreeGLUT REQUIRED)
message(STATUS "GLUT_INCLUDE_DIRS: ${GLUT_INCLUDE_DIRS}")
message(STATUS "GLUT_LIBRARIES: ${GLUT_LIBRARIES}")

# Find ImGui package
find_package(imgui CONFIG REQUIRED)
message(STATUS "ImGui found: ${imgui_FOUND}")

# Enable testing
#enable_testing()
#include(FetchContent)
#FetchContent_Declare(
#    googletest
#    GIT_REPOSITORY https://github.com/google/googletest.git
#    GIT_TAG release-1.12.1
#)
#FetchContent_MakeAvailable(googletest)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
    ${VCPKG_INSTALLED_DIR}/include
)

# Main executable
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(GPUGraphicsProject ${SOURCES})

# Link libraries
target_link_libraries(GPUGraphicsProject PRIVATE 
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${GLUT_LIBRARIES}
    imgui::imgui
    opengl32
)

# Copy shader files to build directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Debug/shaders)
file(COPY ${CMAKE_SOURCE_DIR}/shaders/vertex_shader.glsl DESTINATION ${CMAKE_BINARY_DIR}/Debug/shaders)
file(COPY ${CMAKE_SOURCE_DIR}/shaders/fragment_shader.glsl DESTINATION ${CMAKE_BINARY_DIR}/Debug/shaders)

# Add tests directory
#add_subdirectory(tests)

# Test executable
#file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
#add_executable(renderer_tests ${TEST_SOURCES})
#target_link_libraries(renderer_tests PRIVATE gtest_main gtest)
#add_test(NAME renderer_tests COMMAND renderer_tests)