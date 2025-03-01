cmake_minimum_required(VERSION 3.10)
project(glad)

# Add the glad library
add_library(glad STATIC src/glad.c)

# Specify include directories for glad
target_include_directories(glad PUBLIC include)