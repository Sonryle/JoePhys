file(GLOB SRC_FILES "src/*.cpp")
file(GLOB HEADER_FILES "include/*.hpp")
add_executable(testbed ${SRC_FILES} ${HEADER_FILES})

target_include_directories(testbed PRIVATE include)
target_link_libraries(testbed PRIVATE JoePhys)
