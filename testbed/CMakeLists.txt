cmake_minimum_required(VERSION 3.12)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 17)
project(testbed)

file(GLOB SRC_FILES "src/*.cpp")
file(GLOB HEADER_FILES "include/*.hpp")
add_executable(testbed ${SRC_FILES} ${HEADER_FILES})
target_include_directories(testbed PRIVATE include)

# add all of our libraries
add_subdirectory(dependencies)
add_subdirectory(${CMAKE_SOURCE_DIR}/../JoePhys ${CMAKE_BINARY_DIR}/JoePhys)

# link our libraries into the project
target_link_libraries(testbed PRIVATE JoePhys)
target_link_libraries(testbed PUBLIC glfw)
target_link_libraries(testbed PUBLIC glad)
target_link_libraries(testbed PUBLIC imgui)

# Set our project as the Visual Studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})