cmake_minimum_required(VERSION 3.10)
project(glm)

# Add the glad library
add_library(glm INTERFACE)

# Specify include directories for glm
target_include_directories(glm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
