// function loader & window manager
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// c++ libraries
#include <iostream>
#include <string>
#include <vector>

// header files for classes
#include <renderer.hpp>
#include <shapes.hpp>

// structs & classes
// -----------------

struct Window
{
	GLFWwindow* handle = nullptr;
	int width = 800;
	int height = 800;
	std::string title = "JoePhys!";
};

extern Window window;
extern Renderer renderer;

// functions
// ---------

void windowResizeCallback(GLFWwindow*, int width, int height);