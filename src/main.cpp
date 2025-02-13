#include "main.hpp"

struct Window
{
	GLFWwindow* handle = nullptr;
	int width = 800;
	int height = 800;
	std::string title = "JoePhys!";
};

Window window;
Renderer renderer;

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
	glfwSetFramebufferSizeCallback(window.handle, windowResizeCallback);


	// Load all openGL functions using glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Initialise renderer
	renderer.init();

	// TEMPORARY CIRCLE TO TEST RENDERER!!!
	Circle temporary_circle;
	temporary_circle.position = glm::vec2(0.0f, 200.0f);
	temporary_circle.radius = 50;
	temporary_circle.colour = glm::vec4(0.3f, 1.0f, 0.3f, 1.0f);
	temporary_circle.layer = 1;

	Circle temporary_circle_two;
	temporary_circle_two.position = glm::vec2(0.0f, -200.0f);
	temporary_circle_two.radius = 50;
	temporary_circle_two.colour = glm::vec4(0.3f, 0.3f, 1.0f, 1.0f);
	temporary_circle_two.layer = 1;

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
void windowResizeCallback(GLFWwindow* window_handle, int width, int height)
{
	// Set gl viewport to be the same resolution as our window
	glViewport(0, 0, width, height);
	// Update window width and height variables accordingly
	window.width = width;
	window.height = height;
	// Update renderer's projection matrix so that coordinate system matches window dimensions
	renderer.updateViewMatrix(width, height);
}

int addCircle(int* circle_pointer)
{
	std::cout << "added a circle!\n";
	renderer.circle_stack.push_back(circle_pointer);
	return (int)renderer.circle_stack.size() - 1;
}

int addLine(int* line_pointer)
{
	renderer.line_stack.push_back(line_pointer);
	return (int)renderer.line_stack.size() - 1;
}

int addSquare(int* square_pointer)
{
	renderer.square_stack.push_back(square_pointer);
	return (int)renderer.square_stack.size() - 1;
}