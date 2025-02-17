#ifndef RENDERER_JP
#define RENDERER_JP

// include openGL function loader
#include <glad/glad.h>

// Shader class handles shader compilation
#include <shader.hpp>

// Image loading class MyClass {
#include <stb/stb_image.h>

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
	unsigned int texture_atlas;
	Shader circle_shader;
	Shader line_shader;
	Shader square_shader;

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
	void renderSquare(Square* square);
	// ----------------------------------------------------
};

#endif
