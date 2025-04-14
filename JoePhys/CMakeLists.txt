file(GLOB SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.hpp")

# Add the Core library
add_library(JoePhys STATIC ${SRC_FILES} ${HEADER_FILES})

# Set include directories for JoePhys 
target_include_directories(JoePhys PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# add all of our libraries
add_subdirectory(dependencies)

# link our libraries into the project
target_link_libraries(JoePhys PUBLIC glfw)
target_link_libraries(JoePhys PUBLIC glad)
target_link_libraries(JoePhys PUBLIC imgui)
