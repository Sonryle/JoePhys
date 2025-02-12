// function loader & window manager
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// c++ libraries
#include <iostream>
#include <string>

// header files for classes
#include <renderer.hpp>

// structs & classes
struct Window
{
	GLFWwindow* handle = nullptr;
	int width = 800;
	int height = 800;
	std::string title = "JoePhys!";
};

Window window;
Renderer renderer;

// functions
void window_resize_callback(GLFWwindow*, int width, int height);

int main()
{
	// Initialise GLFW
	glfwInit();

	// Create window using GLFW
	window.handle = glfwCreateWindow(window.width, window.height, window.title.c_str(), NULL, NULL);
	if (window.handle == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window.handle);

	// Assign window resize callback function
	glfwSetFramebufferSizeCallback(window.handle, window_resize_callback);


	// Load all openGL functions using glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Initialise renderer
	renderer.init();

	// game loop
	while (!glfwWindowShouldClose(window.handle))
	{
		// close if escape key pressed
		if (glfwGetKey(window.handle, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window.handle, true);

		// render scene
		renderer.render();

		// swap buffers and poll input events
		glfwSwapBuffers(window.handle);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// This function is called by GLFW every time the window is resized
void window_resize_callback(GLFWwindow* window_handle, int width, int height)
{
	// Set gl viewport to be the same resolution as our window
	glViewport(0, 0, width, height);
	// Update window width and height variables accordingly
	window.width = width;
	window.height = height;
}