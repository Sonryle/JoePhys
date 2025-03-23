#include <cstdio>	// for "stderr" file path constant
#include <algorithm>	// for std::max & std::min

// include glad openGL function loader & GLFW window manager
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// include testbed header files
#include "Renderer.hpp"
#include "Settings.hpp"
#include "SceneManager.hpp"
#include "GUIManager.hpp"

GLFWwindow* jp_window = nullptr;

double time_at_last_render = 0.0f;	// for FPS limit

static void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	camera.WindowResize(width, height);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch(key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_A:
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && action == GLFW_PRESS)
			gui_manager.ToggleAppearanceWindow();
		break;
	case GLFW_KEY_R:
		if (action == GLFW_PRESS)
			scene_manager.SwitchScene(scene_manager.current_scene_number);
		break;
	case GLFW_KEY_LEFT:
		if (action == GLFW_PRESS && scene_manager.current_scene_number != 0)
			scene_manager.SwitchScene(scene_manager.current_scene_number - 1);
		break;
	case GLFW_KEY_RIGHT:
		if (action == GLFW_PRESS && scene_manager.current_scene_number < SCENE_COUNT - 1)
			scene_manager.SwitchScene(scene_manager.current_scene_number + 1);
		break;
	}
}

vec2 cursor_pos(0.0f, 0.0f);
static void MousePosCallback(GLFWwindow* window, double dx, double dy)
{
	ImGuiIO& io = ImGui::GetIO();
	bool left_mouse_button_down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	bool GUI_not_being_interacted_with = !(io.WantCaptureMouse);

	// If the left mouse button is pressed and the GUI is not being interacted with then move the camera by
	// the difference between the new cursor position (dx & dy) and the old cursor position (cursor_pos)
	if (left_mouse_button_down && GUI_not_being_interacted_with)
	{
		vec2 offset(cursor_pos.x - (float)dx, (float)dy - cursor_pos.y);
		camera.center += offset * camera.zoom / 100.0f;
	}
	
	// Update cursor position variable
	cursor_pos.Set(dx, dy);

	return;
}

static void MouseButtonCallback(GLFWwindow*, signed int button, signed int action, signed int mods)
{
	return;
}

static void ScrollCallback(GLFWwindow*, double dx, double dy)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse == 0)
	{
		if (dy < 0)
			camera.zoom = std::max(camera.zoom * 0.95f, 0.00001f);
		else if (dy > 0)
			camera.zoom = std::min(camera.zoom * 1.05f, 1000.0f);
	}

	return;
}

// Initiates GLFW and creates a window
static void InitGLFW()
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
	glfwSetMouseButtonCallback(jp_window, MouseButtonCallback);
	glfwSetCursorPosCallback(jp_window, MousePosCallback);
	glfwSetScrollCallback(jp_window, ScrollCallback);

	// Disable GLFW's VSync
	glfwSwapInterval(0);
}

// Initiates glad, which loads the openGL functions from our graphics cards
static void InitGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{ fprintf(stderr, "Failed to initiate GLAD\n");
		return;
	}
}

void Step()
{
	// Step the world forward
	scene_manager.current_scene->Step();
	
	// Add shapes to be rendered to the renderers stack
	scene_manager.current_scene->Render();
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
			glClear(GL_COLOR_BUFFER_BIT);
		
			// Update Physics Sim
			Step();

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
