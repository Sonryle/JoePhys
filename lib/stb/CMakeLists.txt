cmake_minimum_required(VERSION 3.10)
project(stb)

# Add the glad library
add_library(stb INTERFACE)

# Specify include directories for glm
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
