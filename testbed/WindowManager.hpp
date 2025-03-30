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
	static void InputCallback();

	vec2 cursor_pos;
	GLFWwindow* window;
	GLFWmonitor* monitor;
	const GLFWvidmode* mode;
};

extern WindowManager window_manager;

#endif
