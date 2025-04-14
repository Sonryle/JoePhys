cmake_minimum_required(VERSION 3.10)
project (imgui)

# Collect all .cpp & .h files in imgui
file(GLOB SRC_FILES "*.cpp")
file(GLOB HEADER_FILES "*.h")

# Add the imgui library
add_library(imgui STATIC ${SRC_FILES} ${HEADER_FILES})

# Specify include directories for imgui
target_include_directories(imgui PUBLIC .)
target_include_directories(imgui PUBLIC ../GLFW/include)