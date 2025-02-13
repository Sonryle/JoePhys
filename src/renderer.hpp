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
	unsigned int VAO;
	unsigned int VBO;
	Shader defaultShader;

	void init();
	void render();
};

#endif