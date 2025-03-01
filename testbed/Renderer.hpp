#ifndef JP_RENDERER
#define JP_RENDERER

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "JoePhys/Vec2.hpp"

struct Camera
{
	Camera()
	{
		center.set(0.0f, 0.0f);
		zoom = 1.0f;
		window_width = 0;
		window_height = 0;
	}

	vec2 center;
	unsigned int window_width;
	unsigned int window_height;
	float zoom;
};

struct Renderer
{
	/* Renderer(); */
	/* ~Renderer(); */

	void Create();
	/* void Destroy(); */
};

extern Renderer renderer;

#endif
