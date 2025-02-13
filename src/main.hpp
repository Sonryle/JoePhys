#ifndef MAIN_JP
#define MAIN_JP

// function loader & window manager
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// c++ libraries
#include <iostream>
#include <string>
#include <vector>

// header files for classes
#include <shapes.hpp>
#include <renderer.hpp>

// functions
// ---------

void windowResizeCallback(GLFWwindow*, int width, int height);

int addCircle(int* circle_pointer);
int addLine(int* line_pointer);
int addSquare(int* square_pointer);

#endif