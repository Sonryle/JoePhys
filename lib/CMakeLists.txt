add_library(
	Core STATIC
	include/Core/Core.hpp
	src/Core/Core.cpp
)

target_include_directories(Core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# We set a compile definition for our Core library. It is just a #define which can be
# accessed by everything in the library. Since it is public, any project using the Core
# library can access it too.
target_compile_definitions(Core PUBLIC CORE_VERSION=1)
