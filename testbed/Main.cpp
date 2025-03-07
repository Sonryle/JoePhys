#include <cstdio>	// for "stderr" file path constant
#include <algorithm>	// for std::max & std::min

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.hpp"
#include "Settings.hpp"
#include "GUI.hpp"

GLFWwindow* window = nullptr;
Settings settings;

double cursor_x = 0;
double cursor_y = 0;
bool first_cursor_movement = 1;

static void glfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error has occurred (Error Code: %d) Description: \"%s\".\n", error, description);
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	camera.WindowResize(width, height);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch(key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_A:
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && action == GLFW_PRESS)
			appearanceWindowShown = (appearanceWindowShown == 1)? 0 : 1;
		break;
	}
}

static void mousePosCallback(GLFWwindow* window, double dx, double dy)
{
	// Update Camera Position
	// (if it is not the first frame that the cursor is moving
	//  over the screen and ImGui window is not being interacted with)
	ImGuiIO& io = ImGui::GetIO();
	if ( !(first_cursor_movement || io.WantCaptureMouse) )
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			camera.center += vec2(cursor_x - dx, dy - cursor_y) * camera.zoom;

	cursor_x = dx;
	cursor_y = dy;
	first_cursor_movement = 0;
		
	return;
}

static void mouseButtonCallback(GLFWwindow*, signed int button, signed int action, signed int mods)
{
	return;
}

static void scrollCallback(GLFWwindow*, double dx, double dy)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse == 0)
	{
		if (dy < 0)
			camera.zoom = std::max(camera.zoom * 0.95f, 0.1f);
		else if (dy > 0)
			camera.zoom = std::min(camera.zoom * 1.05f, 10.0f);
	}

	return;
}

static void initGLFW()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(settings.initial_window_width, settings.initial_window_height, "JoePhys", NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

	glfwSetErrorCallback(glfwErrorCallback);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, mousePosCallback);
	glfwSetScrollCallback(window, scrollCallback);
}

static void initGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{ fprintf(stderr, "Failed to initiate GLAD\n");
		return;
	}
}

void step()
{
	renderer.AddColourTest();
}

int main()
{
	initGLFW();
	initGlad();
	initImGui(window);

	camera.Create(settings.initial_window_width, settings.initial_window_height);
	renderer.Create();

	/* switchScene(0); */

	while(!glfwWindowShouldClose(window))
	{
		// Clear Screen
		colour* bg = &palette.colours[settings.scene_colours.background];
		glClearColor(bg->r, bg->g, bg->b, bg->a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update Physics Sim
		step();

		// Render Frame
		renderer.Flush();
		updateImGui();
		glfwSwapBuffers(window);

		// Poll Window Events
		glfwPollEvents();
	}

	renderer.Destroy();
	glfwTerminate();

	return 1;
}
