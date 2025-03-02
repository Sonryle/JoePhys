#include <cstdio>	// for "stderr" file path constant

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "Renderer.hpp"
#include "Settings.hpp"

// TEMPORARY
#include "JoePhys/Vec2.hpp"
#include "Colours.hpp"

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
	camera.window_width = width;
	camera.window_height = height;
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

static void mousePosCallback(GLFWwindow* window, double dx, double dy)
{
	// Update Camera Position
	// (if ImGui window is not focused, and it is not the first frame
	//  that the cursor is moving over the screen)
	if (first_cursor_movement == 0 && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) == 0)
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			camera.center += vec2(cursor_x - dx, dy - cursor_y);

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
	if (dy < 0)
		camera.zoom = std::max(camera.zoom * 0.95f, 0.1f);
	else if (dy > 0)
		camera.zoom = std::min(camera.zoom * 1.05f, 10.0f);

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
	{
		fprintf(stderr, "Failed to initiate GLAD\n");
		return;
	}
}

static void initImGui()
{
	ImGui::CreateContext();	

	ImGui_ImplGlfw_InitForOpenGL(window, 1);
	ImGui_ImplOpenGL3_Init();
}

static void updateImGui()
{
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void step()
{
	// TEMPORARY RENDER TRIANGLE FUNCTION
	vec2 t1p1( 200.0f, -150.0f  );
	vec2 t1p2(-200.0f, -150.0f);
	vec2 t1p3(   0.0f,  150.0f );
	colour t1col(0.3f, 0.4f, 1.0f, 1.0f);

	vec2 t2p1( 295.0f, -940.0f  );
	vec2 t2p2( 194.0f,  048.0f);
	vec2 t2p3(   0.0f,  190.0f );
	colour t2col(0.3f, 0.1f, 0.4f, 1.0f);
	renderer.RenderTriangle(t1p1, t1p2, t1p3, t1col);
	renderer.RenderTriangle(t2p1, t2p2, t2p3, t2col);
}

int main()
{
	initGLFW();
	initGlad();
	initImGui();

	camera.Create(settings.initial_window_width, settings.initial_window_height);
	renderer.Create();

	glClearColor(0.9f, 0.3f, 0.3f, 1.0f);

	while(!glfwWindowShouldClose(window))
	{
		// Clear Screen
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
