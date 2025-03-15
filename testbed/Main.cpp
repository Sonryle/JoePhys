#include <cstdio>	// for "stderr" file path constant
#include <algorithm>	// for std::max & std::min

// include glad openGL function loader & GLFW window manager
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// include testbed header files
#include "Renderer.hpp"
#include "Settings.hpp"
#include "Scene.hpp"
#include "AllSceneHeaders.hpp"
#include "GUI.hpp"

GLFWwindow* window = nullptr;

real cursor_x = 0;
real cursor_y = 0;
bool first_cursor_movement = 1;

Scene* current_scene = nullptr;
void SwitchScene(int scene_number);

double time_at_last_render = 0.0f;	// for FPS limit

// function to switch between scenes
void SwitchScene(int scene_number)
{
	delete current_scene;
	switch (scene_number)
	{
	case 0:
		current_scene = new CollisionScene();
		break;
	case 1:
		current_scene = new PinballScene();
		break;
	case 2:
		current_scene = new SquishySquareScene();
		break;
	case 3:
		current_scene = new ColourDebugScene();
		break;
	default:
		SwitchScene(0);
		break;
	}
}

// set up all of the callback functions

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
	case GLFW_KEY_R:
		if (action == GLFW_PRESS)
			SwitchScene(settings.scene_number);
		break;
	case GLFW_KEY_A:
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && action == GLFW_PRESS)
			appearanceWindowHidden = (appearanceWindowHidden == 1)? 0 : 1;
		break;
	case GLFW_KEY_LEFT:
		if (action == GLFW_PRESS && settings.scene_number != 0)
		{
			settings.scene_number -= 1;
			settings.scene_has_changed = 1;
		}
		break;
	case GLFW_KEY_RIGHT:
		if (action == GLFW_PRESS && settings.scene_number + 1 < SCENE_COUNT)
		{
			settings.scene_number += 1;
			settings.scene_has_changed = 1;
		}
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
			camera.center += vec2(cursor_x - (float)dx, (float)dy - cursor_y) * camera.zoom;

	cursor_x = (float)dx;
	cursor_y = (float)dy;
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
			camera.zoom = std::max(camera.zoom * 0.95f, 0.001f);
		else if (dy > 0)
			camera.zoom = std::min(camera.zoom * 1.05f, 10.0f);
	}

	return;
}

// Initiates GLFW and creates a window
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

// Initiates glad, which loads the openGL functions from our graphics cards
static void initGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{ fprintf(stderr, "Failed to initiate GLAD\n");
		return;
	}
}

void Step()
{
	// Switch scene if settings says to do so
	if (settings.scene_has_changed)
	{
		SwitchScene(settings.scene_number);
		settings.scene_has_changed = 0;
	}

	// Step the world forward
	current_scene->Step();
	
	// Render current scene
	current_scene->Render();
}

int main()
{
	// initiate GLFW, glad and imgui
	initGLFW();
	initGlad();
	initImGui(window);

	// Disable GLFW's VSync
	glfwSwapInterval(0);

	// Set scene
	SwitchScene(0);

	// Create our camera and renderer
	camera.Create(settings.initial_window_width, settings.initial_window_height);
	renderer.Create();

	// main loop
	while(!glfwWindowShouldClose(window))
	{
		// Implement Frame Limit
		double current_time = glfwGetTime();
		if (current_time - time_at_last_render > (1.0f / settings.frame_limit))
		{
			// Clear Screen
			colour* bg = &palette.colours[settings.scene_colours.background];
			glClearColor(bg->r, bg->g, bg->b, bg->a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
			// Update Physics Sim
			Step();

			// Render Frame
			renderer.Flush();
			DrawGui();
			glfwSwapBuffers(window);

			// Update time at last render
			time_at_last_render = current_time;
		}

		// Poll Window Events
		glfwPollEvents();
	}

	// destroy renderer & terminate GLFW
	renderer.Destroy();
	glfwTerminate();

	return 1;
}
