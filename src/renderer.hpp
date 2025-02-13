#ifndef RENDERER_JP
#define RENDERER_JP

#include <iostream>
#include <vector>

// include openGL function loader
#include <glad/glad.h>

// Shader class handles shader compilation
#include "shader.hpp"

class Renderer
{
public:

	// variables
	// ---------

	std::vector<int*> circle_stack;
	std::vector<int*> line_stack;
	std::vector<int*> square_stack;
	glm::mat4 projection_matrix;
	unsigned int VAO;
	unsigned int VBO;
	Shader defaultShader;

	// FUNCTIONS
	// ---------

	void init();
	void render();
	void updateViewMatrix(int new_width, int new_height);
};

#endif