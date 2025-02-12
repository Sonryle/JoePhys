cmake_minimum_required(VERSION 3.12)
project(JoePhys)

# create our application
add_executable(JoePhys src/main.cpp)

# build all of our libraries
add_subdirectory(lib)

# link our libraries into JoePhyss
target_link_libraries(JoePhys PUBLIC glfw)
target_link_libraries(JoePhys PUBLIC glad)

# Set our project as the Visual Studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})