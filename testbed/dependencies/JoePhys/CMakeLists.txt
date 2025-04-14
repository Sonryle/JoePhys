file(GLOB SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.hpp")

# Add the Core library
add_library(JoePhys STATIC ${SRC_FILES} ${HEADER_FILES})

# Set include directories for JoePhys 
target_include_directories(JoePhys PUBLIC include)
