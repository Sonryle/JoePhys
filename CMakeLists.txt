cmake_minimum_required(VERSION 3.12)
project(JoePhys)

# add all of our libraries
add_subdirectory(lib)

# create our application and link it to our libraries
add_executable(JoePhys src/main.cpp)
target_link_libraries(JoePhys PUBLIC glfw)

# Set our project as the Visual Studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})