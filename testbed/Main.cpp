// include glad openGL function loader & GLFW window manager
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// include testbed header files
#include "Renderer.hpp"
#include "Settings.hpp"
#include "SceneManager.hpp"
#include "GUIManager.hpp"
#include "imgui.h"

GLFWwindow* jp_window = nullptr;
GLFWmonitor* monitor = nullptr;
const GLFWvidmode* mode = nullptr;
bool isFullscreen = false;

Particle* selected_particle = nullptr;
bool selected_particle_is_static = 0;

double time_at_last_render = 0.0f;
vec2 cursor_pos(0.0f, 0.0f);


void toggleFullscreen()
{
	isFullscreen = !isFullscreen;
	if (isFullscreen)
	{
		monitor = glfwGetPrimaryMonitor();
		mode = glfwGetVideoMode(monitor);

		glfwSetWindowMonitor(jp_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else
	{
		glfwSetWindowMonitor(jp_window, nullptr, 0, 0, 800, 800, 0);
	}
}

// Is called when window is resized
static void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	camera.WindowResize(width, height);
}

// Is called when a key is pressed
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch(key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_SPACE:
		if (glfwGetKey(jp_window, GLFW_KEY_LEFT_CONTROL) && action == GLFW_PRESS)
			toggleFullscreen();
		break;
	case GLFW_KEY_A:
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && action == GLFW_PRESS)
			gui_manager.ToggleAppearanceWindow();
		break;
	case GLFW_KEY_S:
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && action == GLFW_PRESS)
			gui_manager.ToggleSimulationWindow();
		break;
	case GLFW_KEY_R:
		if (action == GLFW_PRESS)
			if (glfwGetKey(jp_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				scene_manager.SwitchScene(scene_manager.current_scene_number);
		break;
	case GLFW_KEY_LEFT:
		if (action == GLFW_PRESS && scene_manager.current_scene_number != 0)
			if (glfwGetKey(jp_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				scene_manager.SwitchScene(scene_manager.current_scene_number - 1);
		break;
	case GLFW_KEY_RIGHT:
		if (action == GLFW_PRESS && scene_manager.current_scene_number < SCENE_COUNT - 1)
			if (glfwGetKey(jp_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				scene_manager.SwitchScene(scene_manager.current_scene_number + 1);
		break;
	}
}

// Is called when mouse position changes
static void MousePosCallback(GLFWwindow* window, double dx, double dy)
{
	if (!ImGui::GetIO().WantCaptureMouse)
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			if (glfwGetKey(jp_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				camera.center += vec2(cursor_pos.x - dx, dy - cursor_pos.y) * camera.zoom / 100.0f;
	cursor_pos.Set(dx, dy);
}

// Is called when mouse wheel is scrolled
static void ScrollCallback(GLFWwindow*, double dx, double dy)
{
	if (ImGui::GetIO().WantCaptureMouse == 0 && glfwGetKey(jp_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		if (dy < 0)
			camera.zoom = std::max(camera.zoom * 0.95f, 0.00001f);
		else
			camera.zoom = std::min(camera.zoom * 1.05f, 1000.0f);
	}
}

// Is called every frame
static void FrameCallback()
{
	// If the 'P' key & left CTRL pressed, add static particles into the scene at that position of the cursor
	if (glfwGetKey(jp_window, GLFW_KEY_P) == GLFW_PRESS)
		scene_manager.current_scene->AddStaticParticle(camera.ScreenSpaceToWorldSpace(cursor_pos), 0.1f);

	// If the 'R' key & left CTRL are pressed, add add a repulsion force at the location of the mouse pointer
	if (glfwGetKey(jp_window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(jp_window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS)
		scene_manager.current_scene->AddRepulsionForce(camera.ScreenSpaceToWorldSpace(cursor_pos), settings.repulsion_tool_strength);

	// If the 'A' key & left CTRL are pressed, add add a repulsion force at the location of the mouse pointer
	if (glfwGetKey(jp_window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(jp_window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS)
		scene_manager.current_scene->AddAttractionForce(camera.ScreenSpaceToWorldSpace(cursor_pos), settings.attraction_tool_strength);

	// If the left mouse button is pressed & left CTLR ISN'T pressed, select particle closest to the cursor,
	// move it to the mouse cursor and set it to be static while it is being held
	if (ImGui::GetIO().WantCaptureMouse == 0 && glfwGetMouseButton(jp_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && glfwGetKey(jp_window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS)
	{
		if (selected_particle == nullptr)
		{
			selected_particle = scene_manager.current_scene->GetNearestParticle(camera.ScreenSpaceToWorldSpace(cursor_pos));
			selected_particle_is_static = selected_particle->is_static;
		}
		if (selected_particle != nullptr)
		{
			selected_particle->is_static = 1;
			scene_manager.current_scene->MoveParticle(selected_particle, camera.ScreenSpaceToWorldSpace(cursor_pos));
		}
	}
	else
	{
		if (selected_particle != nullptr)
		{
			selected_particle->is_static = selected_particle_is_static;
			selected_particle = nullptr;
		}
	}

	return;
}

// Initiates GLFW and creates a window
static void InitGLFW()
{
	if (!glfwInit())
		fprintf(stderr, "Failed to initialize GLFW\n");

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	jp_window = glfwCreateWindow(settings.initial_window_width, settings.initial_window_height, "JoePhys", NULL, NULL);

	if (!jp_window)
	{
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(jp_window);
	glfwSetFramebufferSizeCallback(jp_window, FramebufferResizeCallback);
	glfwSetKeyCallback(jp_window, KeyCallback);
	glfwSetCursorPosCallback(jp_window, MousePosCallback);
	glfwSetScrollCallback(jp_window, ScrollCallback);
	glfwSwapInterval(0);
}

// Initiates glad, which loads the openGL functions from the graphics card
static void InitGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		fprintf(stderr, "Failed to initiate GLAD\n");
}

void Step()
{
	// Step the world forward
	scene_manager.current_scene->Step();

	// Add the shapes to be rendered onto the renderer's stack
	scene_manager.current_scene->Render(jp_window, cursor_pos);
}

// Program entry point
int main()
{
	// initiate GLFW, glad and imgui
	InitGLFW();
	InitGlad();
	gui_manager.Init(jp_window);

	// Set scene
	scene_manager.SwitchScene(0);

	// Create our camera and renderer
	camera.Create(settings.initial_window_width, settings.initial_window_height);
	renderer.Create();

	// main loop
	while(!glfwWindowShouldClose(jp_window))
	{
		// Implement Frame Limit
		double current_time = glfwGetTime();
		if (current_time - time_at_last_render > (1.0f / settings.frame_limit))
		{
			// Clear Screen
			colour bg = palette.colours[scene_manager.current_scene->colours.background];
			glClearColor(bg.r, bg.g, bg.b, bg.a);
			glClear(GL_COLOR_BUFFER_BIT);
		
			// Update Physics Sim
			Step();

			// Manage Input
			FrameCallback();

			// Render Frame
			renderer.Flush();
			gui_manager.DrawGui();
			glfwSwapBuffers(jp_window);

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
