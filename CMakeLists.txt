cmake_minimum_required(VERSION 3.12)
project(JoePhys)

# make it so that CMake always generates a compile_commands.json file (for neovim coc)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# create our application
add_executable(
JoePhys
src/main.cpp
src/clock.cpp
src/clock.hpp
src/shapes.cpp
src/shapes.hpp
src/shader.cpp
src/shader.hpp
src/renderer.cpp
src/renderer.hpp
res/shaders/line_shader.frag
res/shaders/line_shader.vert
res/shaders/circle_shader.frag
res/shaders/circle_shader.vert
res/shaders/square_shader.frag
res/shaders/square_shader.vert
)

# add /src as an include directory
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC src)

# build all of our libraries
add_subdirectory(lib)

# link our libraries into JoePhyss
target_link_libraries(JoePhys PUBLIC glfw)
target_link_libraries(JoePhys PUBLIC glad)
target_link_libraries(JoePhys PUBLIC glm)
target_link_libraries(JoePhys PUBLIC stb)

# Set our project as the Visual Studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})

# copy "res" folder into the build output directory (and the build directory) every time project is built
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/res" "${CMAKE_BINARY_DIR}/res"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/res" "${CMAKE_BINARY_DIR}/$<CONFIG>/res"

    COMMENT "Copying resource folder to both build/res and build/CurrentConfig/res"
)
