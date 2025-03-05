#include <cstdio>	// for "stderr" file path constant
#include <algorithm>	// for std::max & std::min

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "JoePhys/Circle.hpp"
#include "JoePhys/PhysObject.hpp"
#include "JoePhys/World.hpp"

#include "Renderer.hpp"
#include "Settings.hpp"
#include "GUI.hpp"

GLFWwindow* window = nullptr;
Settings settings;

double cursor_x = 0;
double cursor_y = 0;
bool first_cursor_movement = 1;

// TEMPORARY CODE HERE DO NOT THINK THAT I WILL LET THIS SLIDE
World* world;

static void glfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error has occurred (Error Code: %d) Description: \"%s\".\n", error, description);
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	camera.window_width = (float)width;
	camera.window_height = (float)height;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch(key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_A:
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && action == GLFW_PRESS)
			appearanceWindowShown = (appearanceWindowShown == 1)? 0 : 1;
		else if (action == GLFW_PRESS)
			world->RemoveAllPhysObjects();
		break;
	case GLFW_KEY_D:
		if (action == GLFW_PRESS)
			if (world->getPhysObjects().size() > 0)
				world->RemovePhysObject(world->getPhysObjects()[0]);
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
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse == 0)
	{
		if (dy < 0)
			camera.zoom = std::max(camera.zoom * 0.95f, 0.1f);
		else if (dy > 0)
			camera.zoom = std::min(camera.zoom * 1.05f, 10.0f);
	}

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
	{ fprintf(stderr, "Failed to initiate GLAD\n");
		return;
	}
}

void step()
{
	// TEMPORARILY CREATE CIRCLES
	std::vector<PhysObject*> v = world->getPhysObjects();

	// loop over objects in world and draw them
	for (int i = 0; i < (int)v.size(); i++)
	{
		Circle* c = (Circle*)v[i]->shape;
		renderer.AddSolidCircle(v[i]->position, c->radius, settings.circle_res, palette.yellow, palette.dark_yellow);
	}
}

int main()
{
	initGLFW();
	initGlad();
	initImGui(window);

	// TEMPORARY CODE
	world = new World();
	Circle c(50.0f);
	PhysObject* one = new PhysObject(&c, vec2(300.0f, 300.0f), 1.0f);
	PhysObject* two = new PhysObject(&c, vec2(300.0f, 0.0f), 1.0f);
	PhysObject* thr = new PhysObject(&c, vec2(300.0f, -300.0f), 1.0f);
	PhysObject* one1 = new PhysObject(&c, vec2(0.0f, 300.0f), 1.0f);
	PhysObject* two1 = new PhysObject(&c, vec2(0.0f, 0.0f), 1.0f);
	PhysObject* thr1 = new PhysObject(&c, vec2(0.0f, -300.0f), 1.0f);
	PhysObject* one11 = new PhysObject(&c, vec2(-300.0f, 300.0f), 1.0f);
	PhysObject* two11 = new PhysObject(&c, vec2(-300.0f, 0.0f), 1.0f);
	PhysObject* thr11 = new PhysObject(&c, vec2(-300.0f, -300.0f), 1.0f);
	world->AddPhysObject(one);
	world->AddPhysObject(two);
	world->AddPhysObject(thr);
	world->AddPhysObject(one1);
	world->AddPhysObject(two1);
	world->AddPhysObject(thr1);
	world->AddPhysObject(one11);
	world->AddPhysObject(two11);
	world->AddPhysObject(thr11);

	camera.Create(settings.initial_window_width, settings.initial_window_height);
	renderer.Create();

	while(!glfwWindowShouldClose(window))
	{
		// Clear Screen
		colour* bg = settings.scene_colours.background;
		glClearColor(bg->r, bg->g, bg->b, bg->a);
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
