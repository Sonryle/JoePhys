#include <cstdio>	// for "stderr" file path constant

#ifdef DEBUG
#include <iostream>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.hpp"
#include "Settings.hpp"

GLFWwindow* window = nullptr;
Settings settings;

static void glfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error has occurred (Error Code: %d) Description: \"%s\".\n", error, description);
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch(key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	}
}

static void mouseMotionCallback(GLFWwindow* window, double dx, double dy)
{
	return;
}

static void mouseButtonCallback(GLFWwindow*, signed int button, signed int action, signed int mods)
{
	return;
}

static void scrollCallback(GLFWwindow*, double dx, double dy)
{
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
	glfwSetCursorPosCallback(window, mouseMotionCallback);
	glfwSetScrollCallback(window, scrollCallback);
}

static void initGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initiate GLAD\n");
		return;
	}
}

void step()
{
	// TEMPORARY RENDER TRIANGLE FUNCTION
	renderer.RenderDefaultTriangle();
	renderer.Flush();
}

int main()
{
	initGLFW();
	initGlad();

	renderer.Create();

	glClearColor(0.9f, 0.3f, 0.3f, 1.0f);

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		step();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 1;
}
