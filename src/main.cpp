// C++ libraries
#include "glm/detail/type_vec.hpp"
#include <stdio.h>
#include <string.h>

// OpenGL function loader
#include <glad/glad.h>

// Window Manager
#include <GLFW/glfw3.h>

// JoePhys Headers
#include <clock.hpp>
#include <renderer.hpp>
#include <shapes.hpp>
#include <ui.hpp>

// functions
void windowResizeCallback(GLFWwindow*, int, int);

struct Window
{
	GLFWwindow* handle = nullptr;
	int width = 800;
	int height = 800;
	std::string title = "JoePhys! (Use WASD + Up Down Left & Right)";
};

Clock jp_clock;
Window window;
Renderer renderer;

int main()
{
	// Initialise GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window using GLFW
	window.handle = glfwCreateWindow(window.width, window.height, window.title.c_str(), NULL, NULL);
	if (window.handle == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window.handle);
	glfwSwapInterval(1); // Disable VSync

	// Assign window resize callback function
	glfwSetFramebufferSizeCallback(window.handle, windowResizeCallback);

	// Load all openGL functions using glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialise renderer
	renderer.init();

	// TEMPORARY SHAPES
	Circle tempCirc;
	tempCirc.layer = 1;
	tempCirc.position = glm::vec2(0.0f, 0.0f);
	tempCirc.radius = 50;
	tempCirc.colour = glm::vec4(0.3f, 1.0f, 0.3f, 1.0f);

	Circle tempCircTwo;
	tempCircTwo.layer = 1;
	tempCircTwo.position = glm::vec2(0.0f, 0.0f);
	tempCircTwo.radius = 50;
	tempCircTwo.radius = 50;
	tempCircTwo.colour = glm::vec4(0.3f, 1.0f, 0.3f, 1.0f);

	Line tempLine;
	tempLine.layer = 0;
	tempLine.layer = 0;
	tempLine.thickness = 40;
	tempLine.start_position = glm::vec2(100.0f, -200.0f);
	tempLine.end_position = glm::vec2(-100.0f, 200.0f);
	tempLine.colour = glm::vec4(0.3f, 0.3f, 1.0f, 1.0f);

	// myles ui testing
	UI tempUI;
	tempUI.layer = 100;
	tempUI.x_scale = 400;
	tempUI.y_scale = 200;
	tempUI.offset = 30;
	tempUI.bezel = 30;
	tempUI.bezelThickness = 10;
	tempUI.position = glm::vec2((window.width / 2.0f) - (tempUI.x_scale / 2.0f), (window.height / 2.0f)- (tempUI.y_scale / 2.0f));
	tempUI.colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	tempUI.bezelColour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	// game loop
	while (!glfwWindowShouldClose(window.handle))
	{
		// should close window?
		// --------------------
		if (glfwGetKey(window.handle, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window.handle, true);

		// Update our clock
		// ----------------
		jp_clock.update();

		// myles UI
		tempUI.position = glm::vec2((window.width / 2.0f) - (tempUI.x_scale / 2.0f), (window.height / 2.0f)- (tempUI.y_scale / 2.0f));

		// render scene
		// ------------

		// set background to be red
		glClearColor(0.92f, 0.28f, 0.37f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render objects	(objects must be rendered from back to front)
		renderer.renderLine(&tempLine);
		renderer.renderCircle(&tempCirc);	// circles must always be rendered last
		renderer.renderCircle(&tempCircTwo);	// since they have some transparent fragments
		renderer.renderUI(&tempUI);

		// TEMPORARY LINE MOVEMENT CODE
		// ----------------------------
		
		float speed = 240;
		if (glfwGetKey(window.handle, GLFW_KEY_UP))
			tempLine.end_position = glm::vec2(tempLine.end_position.x, tempLine.end_position.y + speed * jp_clock.deltaTime);
		if (glfwGetKey(window.handle, GLFW_KEY_DOWN))
			tempLine.end_position = glm::vec2(tempLine.end_position.x, tempLine.end_position.y - speed * jp_clock.deltaTime);

		if (glfwGetKey(window.handle, GLFW_KEY_LEFT))
			tempLine.end_position = glm::vec2(tempLine.end_position.x - speed * jp_clock.deltaTime, tempLine.end_position.y);
		if (glfwGetKey(window.handle, GLFW_KEY_RIGHT))
			tempLine.end_position = glm::vec2(tempLine.end_position.x + speed * jp_clock.deltaTime, tempLine.end_position.y);

		if (glfwGetKey(window.handle, GLFW_KEY_W))
			tempLine.start_position = glm::vec2(tempLine.start_position.x, tempLine.start_position.y + speed * jp_clock.deltaTime);
		if (glfwGetKey(window.handle, GLFW_KEY_S))
			tempLine.start_position = glm::vec2(tempLine.start_position.x, tempLine.start_position.y - speed * jp_clock.deltaTime);

		if (glfwGetKey(window.handle, GLFW_KEY_A))
			tempLine.start_position = glm::vec2(tempLine.start_position.x - speed * jp_clock.deltaTime, tempLine.start_position.y);
		if (glfwGetKey(window.handle, GLFW_KEY_D))
			tempLine.start_position = glm::vec2(tempLine.start_position.x + speed * jp_clock.deltaTime, tempLine.start_position.y);

		tempCirc.position = tempLine.start_position;
		tempCircTwo.position = tempLine.end_position;

		// swap buffers and poll input events
		// ----------------------------------
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
