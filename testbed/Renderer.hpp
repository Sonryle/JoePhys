#ifndef JP_RENDERER
#define JP_RENDERER

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "JoePhys/Vec2.hpp"

struct GLRenderLines;
struct GLRenderTriangles;

struct Camera
{
	Camera()
	{
		center.set(0.0f, 0.0f);
		zoom = 1.0f;
		window_width = 0;
		window_height = 0;
	}

	void GenerateProjectionMatrix(float p[16]);

	vec2 center;
	unsigned int window_width;
	unsigned int window_height;
	float zoom;
};

struct Renderer
{
	Renderer();
	~Renderer();

	void Create();
	void Destroy();

	void RenderDefaultTriangle();

	void Flush();

	GLRenderLines* lines = nullptr;
	GLRenderTriangles* triangles = nullptr;
};

extern Camera camera;
extern Renderer renderer;

#endif
