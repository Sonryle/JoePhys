#include <cstdio>	// for "stderr" file path constant

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.hpp"
#include "Settings.hpp"
#include "GUI.hpp"

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
	// (if it is not the first frame that the cursor is moving
	//  over the screen and ImGui window is not being interacted with)
	ImGuiIO& io = ImGui::GetIO();
	if (first_cursor_movement == 0 && io.WantCaptureMouse == 0)
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
	// TOP ROW OF TRIANGLES
	// --------------------
	
	// tri 1
	vec2 tt1p1( -390,  0 );
	vec2 tt1p2( -310,  0 );
	vec2 tt1p3( -350, 65 );
	colour tt1col(palette.dark_gray);
	// tri 2
	vec2 tt2p1( -290,  0 );
	vec2 tt2p2( -210,  0 );
	vec2 tt2p3( -250, 65 );
	colour tt2col(palette.dark_red);
	// tri 3
	vec2 tt3p1( -190,   0 );
	vec2 tt3p2( -110,   0 );
	vec2 tt3p3( -150,  65 );
	colour tt3col(palette.dark_green);
	// tri 4
	vec2 tt4p1( -90,   0 );
	vec2 tt4p2( -10,   0 );
	vec2 tt4p3( -50,  65 );
	colour tt4col(palette.dark_yellow);
	// tri 5
	vec2 tt5p1(  10,   0 );
	vec2 tt5p2(  90,   0 );
	vec2 tt5p3(  50,   65 );
	colour tt5col(palette.dark_blue);
	// tri 6
	vec2 tt6p1( 110,   0 );
	vec2 tt6p2( 190,   0 );
	vec2 tt6p3( 150,  65 );
	colour tt6col(palette.dark_purple);
	// tri 7
	vec2 tt7p1( 210,   0 );
	vec2 tt7p2( 290,   0 );
	vec2 tt7p3( 250,  65 );
	colour tt7col(palette.dark_aqua);
	// tri 8
	vec2 tt8p1( 310,   0 );
	vec2 tt8p2( 390,   0 );
	vec2 tt8p3( 350,  65 );
	colour tt8col(palette.dark_white);

	// BOTTOM ROW OF TRIANGLES
	// --------------------
	
	// tri 1
	vec2 bt1p1( -390, -100 );
	vec2 bt1p2( -310, -100 );
	vec2 bt1p3( -350,  -35 );
	colour bt1col(palette.gray);
	// tri 2
	vec2 bt2p1( -290, -100 );
	vec2 bt2p2( -210, -100 );
	vec2 bt2p3( -250,  -35 );
	colour bt2col(palette.red);
	// tri 3
	vec2 bt3p1( -190, -100 );
	vec2 bt3p2( -110, -100 );
	vec2 bt3p3( -150, -35 );
	colour bt3col(palette.green);
	// tri 4
	vec2 bt4p1(  -90, -100 );
	vec2 bt4p2(  -10, -100 );
	vec2 bt4p3(  -50, -35 );
	colour bt4col(palette.yellow);
	// tri 5
	vec2 bt5p1(  10, -100 );
	vec2 bt5p2(  90, -100 );
	vec2 bt5p3(  50, -35 );
	colour bt5col(palette.blue);
	// tri 6
	vec2 bt6p1( 110, -100 );
	vec2 bt6p2( 190, -100 );
	vec2 bt6p3( 150, -35 );
	colour bt6col(palette.purple);
	// tri 7
	vec2 bt7p1( 210, -100 );
	vec2 bt7p2( 290, -100 );
	vec2 bt7p3( 250, -35 );
	colour bt7col(palette.aqua);
	// tri 8
	vec2 bt8p1( 310, -100 );
	vec2 bt8p2( 390, -100 );
	vec2 bt8p3( 350, -35 );
	colour bt8col(palette.white);

	// RENDER TOP TRIANGLES
	renderer.RenderTriangle(tt1p1, tt1p2, tt1p3, tt1col);
	renderer.RenderTriangle(tt2p1, tt2p2, tt2p3, tt2col);
	renderer.RenderTriangle(tt3p1, tt3p2, tt3p3, tt3col);
	renderer.RenderTriangle(tt4p1, tt4p2, tt4p3, tt4col);
	renderer.RenderTriangle(tt5p1, tt5p2, tt5p3, tt5col);
	renderer.RenderTriangle(tt6p1, tt6p2, tt6p3, tt6col);
	renderer.RenderTriangle(tt7p1, tt7p2, tt7p3, tt7col);
	renderer.RenderTriangle(tt8p1, tt8p2, tt8p3, tt8col);
	// RENDER BOTTOM TRIANGLES
	renderer.RenderTriangle(bt1p1, bt1p2, bt1p3, bt1col);
	renderer.RenderTriangle(bt2p1, bt2p2, bt2p3, bt2col);
	renderer.RenderTriangle(bt3p1, bt3p2, bt3p3, bt3col);
	renderer.RenderTriangle(bt4p1, bt4p2, bt4p3, bt4col);
	renderer.RenderTriangle(bt5p1, bt5p2, bt5p3, bt5col);
	renderer.RenderTriangle(bt6p1, bt6p2, bt6p3, bt6col);
	renderer.RenderTriangle(bt7p1, bt7p2, bt7p3, bt7col);
	renderer.RenderTriangle(bt8p1, bt8p2, bt8p3, bt8col);
}

int main()
{
	initGLFW();
	initGlad();
	initImGui(window);

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
