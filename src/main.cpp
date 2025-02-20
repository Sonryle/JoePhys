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
#include <particle_manager.hpp>

// variables
const double FPS_LIMIT = 14;				// the maximum FPS that the program is allowed to reach
const double MINIMUM_FRAME_TIME = 1.0f / FPS_LIMIT;	// the minimum allowed amount of milliseconds between frames
double time_at_last_render = 0.0f;			// what the current time WAS when the previous frame was rendered

// functions
void windowResizeCallback(GLFWwindow*, int, int);

// Structs
struct Window
{
	GLFWwindow* handle = nullptr;
	int width = 1000;
	int height = 1000;
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
		
		// implement frame limit
		if (jp_clock.currentTime - time_at_last_render > MINIMUM_FRAME_TIME)
		{
			// Update our physics simulation
			// -----------------------------

			particle_manager.update();

			// render scene
			// ------------

			// set background to be red
			/* glClearColor(1.0f, 0.94f, 0.67f, 1.0f);	// cream */
			glClearColor(0.21f, 0.22f, 0.23f, 1.0f);	// grey
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// render background objects
			renderer.renderSquare(&graph);

			// render all particles in the particle_stack in our particle manager
			for (int i = 0; i < (int)particle_manager.particle_stack.size(); i++)
				renderer.renderCircle(&particle_manager.particle_stack[i]->circle);

			// TEMPORARILY RENDER ALL LINES IN PARTICLE MANAGER
			for (int i = 0; i < (int)particle_manager.temp_line_stack.size(); i++)
				renderer.renderLine(particle_manager.temp_line_stack[i]);

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
