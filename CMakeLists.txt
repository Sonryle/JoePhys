cmake_minimum_required(VERSION 3.12)
project(MyProject VERSION 1.0.0)

add_subdirectory(lib)

add_executable(App src/App/App.cpp)

target_link_libraries(App PRIVATE Core)