// include testbed header files
#include "WindowManager.hpp"
#include "Renderer.hpp"
#include "Settings.hpp"
#include "SceneManager.hpp"
#include "GUIManager.hpp"

double time_at_last_step = 0.0f;
double current_time = 0.0f;

void Step()
{
	// Manage Input
	glfwPollEvents();
	window_mgr.InputCallback();

	// Step the world forward
	if (!settings.is_paused)
		scene_manager.current_scene->Step();

	// Render Frame
	glClear(GL_COLOR_BUFFER_BIT);
	scene_manager.current_scene->Render();
	renderer.Flush();
	gui_manager.Render();
	glfwSwapBuffers(window_mgr.window);
}

// Program entry point
int main()
{
	// initiate GLFW, glad and imgui
	window_mgr.InitGLFW();
	window_mgr.InitGlad();
	gui_manager.Init(window_mgr.window);

	// Set scene
	scene_manager.SwitchScene(0);

	// Create our camera and renderer
	camera.Create(settings.initial_window_width, settings.initial_window_height);
	renderer.Create();

	// main loop
	while(!glfwWindowShouldClose(window_mgr.window))
	{
		// Implement Frame Limit
		current_time = glfwGetTime();
		if (current_time - time_at_last_step > (1.0f / settings.frame_limit))
		{
			Step();
			time_at_last_step = current_time;
		}
	}

	// destroy renderer & terminate GLFW
	renderer.Destroy();
	glfwTerminate();

	return 1;
}
