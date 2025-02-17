cmake_minimum_required(VERSION 3.10)
project(stb)

# Create a static library for STB
add_library(stb STATIC stb/stb_wrapper.cpp)

# Include directory for stb headers
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
