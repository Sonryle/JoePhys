#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>

// include openGL function loader
#include <glad/glad.h>

// Shader class handles shader compilation
#include "shader.hpp"

class Renderer
{
public:

	unsigned int VAO;
	unsigned int VBO;
	Shader defaultShader;

	void init();
	void render();
};

#endif