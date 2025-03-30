// include testbed header files
#include "WindowManager.hpp"
#include "Renderer.hpp"
#include "Settings.hpp"
#include "SceneManager.hpp"
#include "GUIManager.hpp"

double time_at_last_render = 0.0f;
double current_time = 0.0f;

void Step()
{
	// Step the world forward
	scene_manager.current_scene->Step();

	// Add the shapes to be rendered onto the renderer's stack
	scene_manager.current_scene->Render(window_manager.window, window_manager.cursor_pos);
}

// Program entry point
int main()
{
	// initiate GLFW, glad and imgui
	window_manager.InitGLFW();
	window_manager.InitGlad();
	gui_manager.Init(window_manager.window);

	// Set scene
	scene_manager.SwitchScene(0);

	// Create our camera and renderer
	camera.Create(settings.initial_window_width, settings.initial_window_height);
	renderer.Create();

	// main loop
	while(!glfwWindowShouldClose(window_manager.window))
	{
		// Implement Frame Limit
		current_time = glfwGetTime();
		if (current_time - time_at_last_render > (1.0f / settings.frame_limit))
		{
			// Clear Screen
			glClear(GL_COLOR_BUFFER_BIT);
		
			// Update Physics Sim
			Step();

			// Manage Input
			window_manager.InputCallback();

			// Render Frame
			renderer.Flush();
			gui_manager.DrawGui();
			glfwSwapBuffers(window_manager.window);

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
