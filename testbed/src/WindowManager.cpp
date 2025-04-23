#include "WindowManager.hpp"
#include "GLFW/glfw3.h"
#include "GUIManager.hpp"
#include "SceneManager.hpp"
double PI = 3.141592653589;

WindowManager::WindowManager()
{
	window = nullptr;
	monitor = nullptr;
	mode = nullptr;
	cursor_pos.Set(0.0f, 0.0f);
	window_height = settings.initial_window_height;
	window_width = settings.initial_window_width;
}

// Initiates GLFW and creates a window
void WindowManager::InitGLFW()
{
	if (!glfwInit())
		fprintf(stderr, "Failed to initialize GLFW\n");

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(settings.initial_window_width, settings.initial_window_height, "JoePhys", NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MousePosCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSwapInterval(0);
}

// Initiates glad, which loads the openGL functions from the graphics card
void WindowManager::InitGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		fprintf(stderr, "Failed to initiate GLAD\n");
}

void WindowManager::SetFullscreen(bool is_fullscreen)
{
	if (is_fullscreen)
	{
		monitor = glfwGetPrimaryMonitor();
		mode = glfwGetVideoMode(monitor);

		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else
	{
		glfwSetWindowMonitor(window, nullptr, 50, 50, settings.initial_window_width, settings.initial_window_height, 0);
	}
}

// Is called when window is resized
void WindowManager::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	camera.WindowResize(width, height);
	window_mgr.window_height = height;
	window_mgr.window_height = width;
}

// Is called when a key is pressed
void WindowManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch(key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS && window_mgr.ctrl_pressed)
			window_mgr.SetFullscreen(settings.is_fullscreen = !settings.is_fullscreen);
		break;
	case GLFW_KEY_A:
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && action == GLFW_PRESS)
			gui_manager.appearance_window_shown = !gui_manager.appearance_window_shown;
		break;
	case GLFW_KEY_S:
			if (action == GLFW_PRESS && window_mgr.ctrl_pressed)
				gui_manager.simulation_window_shown = !gui_manager.simulation_window_shown;
			else if (action == GLFW_PRESS && window_mgr.world_exists && !glfwGetMouseButton(window_mgr.window, GLFW_MOUSE_BUTTON_LEFT))
			{
				Particle* p = scene_manager.current_scene->GetNearestParticle(camera.ScreenSpaceToWorldSpace(window_mgr.cursor_pos));
				p->is_static = !p->is_static;
				p->vel.Set(0.0f, 0.0f);
				p->ResetAcceleration();
			}
		break;
	case GLFW_KEY_O:
		if (action == GLFW_PRESS && window_mgr.ctrl_pressed)
			gui_manager.options_window_shown = !gui_manager.options_window_shown;
		break;
	case GLFW_KEY_R:
		if (action == GLFW_PRESS)
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				scene_manager.SwitchScene(scene_manager.current_scene_number);
		break;
	case GLFW_KEY_P:
		if (action != GLFW_RELEASE && !window_mgr.ctrl_pressed)
			scene_manager.current_scene->AddStaticParticle(camera.ScreenSpaceToWorldSpace(window_mgr.cursor_pos), 0.1f);
		if (action == GLFW_PRESS && window_mgr.ctrl_pressed)
			settings.is_paused = !settings.is_paused;
		break;
	case GLFW_KEY_F:
		if (action != GLFW_RELEASE && window_mgr.ctrl_pressed)
			if (settings.is_paused)
				scene_manager.current_scene->Step();
		break;
	case GLFW_KEY_LEFT:
		if (action == GLFW_PRESS && window_mgr.ctrl_pressed)
			if (scene_manager.current_scene_number != 0)
				scene_manager.SwitchScene(scene_manager.current_scene_number - 1);
		break;
	case GLFW_KEY_RIGHT:
		if (action == GLFW_PRESS && window_mgr.ctrl_pressed)
			if (scene_manager.current_scene_number < SCENE_COUNT - 1)
				scene_manager.SwitchScene(scene_manager.current_scene_number + 1);
		break;
	}
}

// Is called when mouse position changes
void WindowManager::MousePosCallback(GLFWwindow* window, double dx, double dy)
{
	if (!ImGui::GetIO().WantCaptureMouse)
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				camera.center += vec2(window_mgr.cursor_pos.x - dx, dy - window_mgr.cursor_pos.y) * camera.zoom / 100.0f;
	window_mgr.cursor_pos.Set(dx, dy);
}

// Is called when mouse wheel is scrolled
void WindowManager::ScrollCallback(GLFWwindow*, double dx, double dy)
{
	if (glfwGetKey(window_mgr.window, GLFW_KEY_LEFT_CONTROL))
	{
		if (ImGui::GetIO().WantCaptureMouse == 0)
		{
			if (dy < 0)
				camera.zoom = std::max(camera.zoom * 0.95f, 0.00001f);
			else
				camera.zoom = std::min(camera.zoom * 1.05f, 1000.0f);
		}
	}
	else
	{
		if (ImGui::GetIO().WantCaptureMouse == 0)
		{
			Particle* p = scene_manager.current_scene->GetNearestParticle(camera.ScreenSpaceToWorldSpace(window_mgr.cursor_pos));
			if (p != nullptr)
			{
				real old_radius = p->radius;
				p->radius += dy / 10.0f;

				if (p->radius < settings.min_particle_size)
					p->radius = settings.min_particle_size;
				if (p->radius > settings.max_particle_size)
					p->radius = settings.max_particle_size;

				// Find old mass to radius
				p->mass = (p->mass / (PI * old_radius * old_radius)) * PI * p->radius * p->radius;
			}
		}
	}
}

// Is called every frame
void WindowManager::InputCallback()
{
	left_mouse_button = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	GUI_focused = ImGui::GetIO().WantCaptureMouse;
	ctrl_pressed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
	world_exists = scene_manager.current_scene->world != nullptr;

	// If the 'R' key & left CTRL are pressed, add add a repulsion force at the location of the mouse pointer
	if (glfwGetKey(window, GLFW_KEY_R) && !ctrl_pressed)
		scene_manager.current_scene->AddRepulsionForce(camera.ScreenSpaceToWorldSpace(cursor_pos), settings.repulsion_tool_radius, settings.repulsion_tool_strength);

	// If the 'A' key & left CTRL are pressed, add add a repulsion force at the location of the mouse pointer
	if (glfwGetKey(window, GLFW_KEY_A) && !ctrl_pressed)
		scene_manager.current_scene->AddAttractionForce(camera.ScreenSpaceToWorldSpace(cursor_pos), settings.attraction_tool_radius, settings.attraction_tool_strength);

	// If the left mouse button is pressed & left CTLR ISN'T pressed, select particle closest to the cursor,
	// move it to the mouse cursor and set it to be static while it is being held
	if (left_mouse_button && !GUI_focused && !ctrl_pressed && world_exists)
	{
		if (scene_manager.current_scene->selected_particle_state == -1)
		{
			scene_manager.current_scene->selected_particle_state = scene_manager.current_scene->selected_particle->is_static;
			scene_manager.current_scene->selected_particle->is_static = 1;
		}
		scene_manager.current_scene->MoveParticle(scene_manager.current_scene->selected_particle, camera.ScreenSpaceToWorldSpace(cursor_pos));
	}
	else
	{
		if (scene_manager.current_scene->selected_particle_state != -1)
		{
			scene_manager.current_scene->selected_particle->is_static = scene_manager.current_scene->selected_particle_state;
			scene_manager.current_scene->selected_particle_state = -1;
			
		}
		scene_manager.current_scene->UpdateSelectedParticle(cursor_pos);
	}

	return;
}

WindowManager window_mgr;
