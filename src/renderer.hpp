#ifndef RENDERER_JP
#define RENDERER_JP

// include openGL function loader
#include <glad/glad.h>

// Shader class handles shader compilation
#include <shader.hpp>

// Include shape classes
#include <shapes.hpp>

class Renderer
{
public:

	// variables
	// ---------

	glm::mat4 projection_matrix;
	unsigned int VAO;
	unsigned int VBO;
	Shader circle_shader;
	Shader line_shader;

	// FUNCTIONS
	// ---------

	void init();
	// ----------------------------------------------------
	void updateViewMatrix(int new_width, int new_height);
	// ----------------------------------------------------
	void renderCircle(Circle* circle);
	// ----------------------------------------------------
	void renderLine(Line* line);
	// ----------------------------------------------------
};

#endif
