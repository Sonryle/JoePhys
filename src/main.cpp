// C++ libraries
#include "glm/detail/type_vec.hpp"
#include <stdio.h>
#include <string.h>
#include <vector>

// OpenGL function loader
#include <glad/glad.h>

// Window Manager
#include <GLFW/glfw3.h>

// JoePhys Headers
#include <clock.hpp>
#include <renderer.hpp>
#include <shapes.hpp>
#include <ui.hpp>
#include <particles.hpp>

// variables
const double FPS_LIMIT = 500;				// the maximum FPS that the program is allowed to reach
const double MINIMUM_FRAME_TIME = 1.0f / FPS_LIMIT;	// the minimum allowed amount of milliseconds between frames
double time_at_last_render = 0.0f;			// what the current time WAS when the previous frame was rendered
bool fullscreen = false; // is the window fullscreen?
bool maximized = false; // is the window maximized?
bool wasF11Pressed = false; // was the F11 key pressed last frame?
bool wasF10Pressed = false; // was the F10 key pressed last frame?

// functions
void windowResizeCallback(GLFWwindow*, int, int);

// Structs
struct Window
{
	GLFWwindow* handle = nullptr;
	int width = 600;
	int height = 600;
	int posX = 0;
	int posY = 0;
	std::string title = "JoePhys!";
};
Window window;

// JoePhys classes
Clock jp_clock;
Renderer renderer;
ParticleManager particle_manager(FPS_LIMIT);

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
	glfwSwapInterval(0); // Disable VSync

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
	renderer.init(window.width, window.height);

	// Create background objects
	Square graph;
	graph.layer = 0;
	graph.x_scale = 1000;
	graph.y_scale = 1000;
	graph.position = glm::vec2(0.0f, 0.0f);
	graph.top_right_tex_coord = glm::vec2(1.0f, 1.0f);
	graph.top_left_tex_coord = glm::vec2(0.0f, 1.0f);
	graph.bottom_right_tex_coord = glm::vec2(1.0f, 0.0f);
	graph.bottom_left_tex_coord = glm::vec2(0.0f, 0.0f);

	// myles ui testing
	UI testUI;
	testUI.layer = 100;
	testUI.x_scale = 350;
	testUI.y_scale = 175;
	testUI.colour = glm::vec4(0.156f, 0.156f, 0.156f, 1.0f);
	testUI.bezelColour = glm::vec4(0.156f, 0.156f, 0.156f, 1.0f);
	testUI.offset = 10;
	testUI.bezel = 30;
	testUI.bezelThickness = 0;
	testUI.alignment = "topRight";

	// set window viewport resolution to window resolution
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window.handle, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    window.width = framebufferWidth;
    window.height = framebufferHeight;
    renderer.updateViewMatrix(window.width, window.height);

	// game loop
	while (!glfwWindowShouldClose(window.handle))
	{
		// fetch window dimensions every frame
		glm::vec2 windowDimensions = glm::vec2(window.width, window.height);
		// should close window?
		// --------------------
		if (glfwGetKey(window.handle, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window.handle, true);

		// should fullscreen?
		// --------------------
		bool isF11Pressed = glfwGetKey(window.handle, GLFW_KEY_F11) == GLFW_PRESS;
		if (isF11Pressed && !wasF11Pressed)
		{
			if (fullscreen)
			{
				glfwSetWindowMonitor(window.handle, nullptr, 0, 0, window.width, window.height, 0);
				fullscreen = false;
			}
			else
			{
				glfwGetWindowSize(window.handle, &window.width, &window.height);
				GLFWmonitor* monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);

				glfwSetWindowMonitor(window.handle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
				fullscreen = true;
			}
		}
		wasF11Pressed = isF11Pressed;
		
		// should maximize?
		// --------------------
		bool (isF10Pressed) = glfwGetKey(window.handle, GLFW_KEY_F10) == GLFW_PRESS;
		if (isF10Pressed && !wasF10Pressed)
		{
			if (maximized)
			{
				glfwRestoreWindow(window.handle);
				maximized = false;
			}
			else
			{
				glfwMaximizeWindow(window.handle);
				maximized = true;
			}
		}
		wasF10Pressed = isF10Pressed;
		
		// Update our clock
		// ----------------
		jp_clock.update();
				
		// implement frame limit
		if (jp_clock.currentTime - time_at_last_render > MINIMUM_FRAME_TIME)
		{
			// Update our physics simulation
			// -----------------------------

			particle_manager.update();
			//particle_manager.setConstraint(glm::vec2(0.0f, 0.0f), window.width, window.height);

			// render scene
			// ------------

			// set background to be red
			glClearColor(0.313f, 0.286f, 0.270f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// render background objects
			renderer.renderSquare(&graph);

			// render all particles in the particle_stack in our particle manager
			for (int i = 0; i < (int)particle_manager.particle_stack.size(); i++)
				renderer.renderCircle(&particle_manager.particle_stack[i]->circle);

			// render stack
			renderer.renderUI(&testUI);
			testUI.updateUI(windowDimensions);
			
			// swap buffers and poll input events
			// ----------------------------------
			glfwSwapBuffers(window.handle);
			glfwPollEvents();

			// update time_at_last_render
			// --------------------------
			time_at_last_render = jp_clock.currentTime;
		}
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
