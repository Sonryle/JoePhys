#ifndef JP_RENDERER
#define JP_RENDERER

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "JoePhys/Vec2.hpp"

struct colour;
struct GLRenderLines;
struct GLRenderTriangles;

struct Camera
{
public:

	Camera()
	{
		center.Set(0.0f, 0.0f);
		zoom = 1.0f;
	}

	void Create(float width, float height)
	{
		WindowResize(width, height);
	}

	void WindowResize(int width, int height)
	{
		window_width = width;
		window_height = height;
	}
	void GenerateProjectionMatrix(float p[16]);

	vec2 center;
	float zoom;

private:
	float window_width;
	float window_height;

private:
};

struct Renderer
{
	Renderer();
	~Renderer();

	void Create();
	void Destroy();

	void AddTriangle(const vec2 p1, const vec2 p2, const vec2 p3, const colour col);
	void AddLine(const vec2 p1, const vec2 p2, const colour col);
	void AddCircle(const vec2 position, const float radius, const unsigned int segments, const colour col);
	void AddSolidCircle(const vec2 position, const float radius, const unsigned int segments, const colour fill_col, const colour outline_col);
	void AddColourTest();

	void Flush();

	GLRenderLines* lines = nullptr;
	GLRenderTriangles* triangles = nullptr;
};

extern Camera camera;
extern Renderer renderer;

#endif
