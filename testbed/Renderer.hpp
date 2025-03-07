#ifndef JP_RENDERER
#define JP_RENDERER

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE	//GLFW INCLUDE NONE to prevent GLFW from including openGL, since we are including it ourselves using glad
#include <GLFW/glfw3.h>

#include "JoePhys/Vec2.hpp"

struct colour;
struct GLRenderLines;
struct GLRenderTriangles;

struct Camera
{
public:
	
	// constructor
	Camera()
	{
		center.Set(0.0f, 0.0f);
		zoom = 1.0f;
	}

	// functions
	void Create(float width, float height) { WindowResize(width, height); }
	void WindowResize(int width, int height) { window_width = width; window_height = height; }
	void GenerateProjectionMatrix(float p[16]);

	vec2 center;
	float zoom;

private:

	// Camera struct requires window width and height
	// so that it can generate the proper projection matrix,
	// keeping the coordinate system fixed nomatter the
	// width or height of the window.
	float window_width;
	float window_height;

private:
};

struct Renderer
{
	// constructor & destructor
	Renderer();
	~Renderer();

	void Create();
	void Destroy();

	// Functions to add shapes to the renderer to be rendered when "Flush()" is called
	void AddTriangle(const vec2 p1, const vec2 p2, const vec2 p3, const colour col);
	void AddLine(const vec2 p1, const vec2 p2, const colour col);
	void AddCircle(const vec2 position, const float radius, const unsigned int segments, const colour col);
	void AddSolidCircle(const vec2 position, const float radius, const unsigned int segments, const colour fill_col, const colour outline_col);
	void AddColourTest();

	// Function which renders the shapes which have been added
	void Flush();

	// Structs which are responsible for rendering lines and triangles
	GLRenderLines* lines = nullptr;
	GLRenderTriangles* triangles = nullptr;
};

// These camera and renderer structs will be accessable by every file which includes "Renderer.hpp".
// They are declared here in "Renderer.hpp" and defined in "Renderer.cpp"
extern Camera camera;
extern Renderer renderer;

#endif
