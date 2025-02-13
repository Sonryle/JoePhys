#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>

// include openGL function loader
#include <glad/glad.h>

// Shader class handles shader compilation
#include "shader.hpp"

// Shapes header file gives us circle, line and square classes
#include "shapes.hpp"


class Renderer
{
public:

	// variables
	// ---------

	std::vector<Circle*> circle_stack;
	std::vector<Line*> line_stack;
	std::vector<Square*> square_stack;
	unsigned int VAO;
	unsigned int VBO;
	Shader defaultShader;

	void init();
	void render();
};

#endif