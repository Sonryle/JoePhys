cmake_minimum_required(VERSION 3.12)
project(JoePhys)

# add all of our libraries
add_subdirectory(lib)

# create our application and link it to our libraries
add_executable(JoePhys src/main.cpp)
target_link_libraries(JoePhys PUBLIC glfw)