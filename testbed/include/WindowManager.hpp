#ifndef JP_WINDOW_MANAGER
#define JP_WINDOW_MANAGER

// include glad openGL function loader & GLFW window manager
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>	// for "stderr" file path constant

#include "Settings.hpp"
#include "Renderer.hpp"
#include "SceneManager.hpp"
#include "GUIManager.hpp"

struct WindowManager
{
	WindowManager();

	void InitGLFW();
	void InitGlad();
	void SetFullscreen(bool is_fullscreen);
	
	// Callback Functions
	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePosCallback(GLFWwindow* window, double dx, double dy);
	static void ScrollCallback(GLFWwindow*, double dx, double dy);
	void InputCallback();

	vec2 cursor_pos;
	int window_width;
	int window_height;
	GLFWwindow* window;
	GLFWmonitor* monitor;
	const GLFWvidmode* mode;

	bool left_mouse_button;
	bool GUI_focused;
	bool ctrl_pressed;
	bool world_exists;

};

extern WindowManager window_mgr;

#endif
