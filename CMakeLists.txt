cmake_minimum_required(VERSION 3.12)
project(JoePhys VERSION 1.0.0)

# make it so that CMake always generates a compile_commands.json file (so neovim can make sense of everything)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(JoePhys)
add_subdirectory(testbed)

# Set App as the default startup project for Visual Studio only
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT testbed)


# cmake_minimum_required(VERSION 3.12)
# project(JoePhys)

# make it so that CMake always generates a compile_commands.json file (so neovim can make sense of everything)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect all .cpp files in src and testbed directories
# file(GLOB SRC_FILES "src/*.cpp")
# file(GLOB TESTBED_FILES "testbed/*.cpp")

# Create the executable
# add_executable(JoePhys ${SRC_FILES} ${TESTBED_FILES})

# add /include as an include directory
# target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include)

# add all of our libraries
# add_subdirectory(dependencies)

# link our libraries into the project
# target_link_libraries(JoePhys PUBLIC glfw)
# target_link_libraries(JoePhys PUBLIC glad)
# target_link_libraries(JoePhys PUBLIC imgui)

# set up definitions
# target_compile_definitions(JoePhys PRIVATE 
    # $<$<CONFIG:Debug>:DEBUG>
    # $<$<CONFIG:Release>:NDEBUG>
    # $<$<CONFIG:RelWithDebInfo>:NDEBUG>
    # $<$<CONFIG:MinSizeRel>:NDEBUG>
# )

# Set our project as the Visual Studio startup project
# set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
