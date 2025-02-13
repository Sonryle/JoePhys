#include "renderer.hpp"

float square_vertex[] = {
	// first triangle
	 400.0f,  400.0f, 0.0f,  // top right
	 400.0f, 0.0f, 0.0f,  // bottom right
	0.0f,  400.0f, 0.0f,  // top left
	// second triangle
	 400.0f, 0.0f, 0.0f,  // bottom right
	0.0f, 0.0f, 0.0f,  // bottom left
	0.0f,  400.0f , 0.0f   // top left
};

void Renderer::init()
{
	std::cout << "initialising renderer!\n";

	// Set up shaders

	defaultShader.init("res/shaders/default_shader.vert", "res/shaders/default_shader.frag");
	defaultShader.use();

	// Set up VAO

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Set up VBO and attach buffer data

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertex), square_vertex, GL_STATIC_DRAW);

	// Set vertex attributes

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set up view matrix
	projection_matrix = glm::ortho(-400.0f, 400.0f, -400.0f, 400.0f, -100.0f, 100.0f);
	defaultShader.setMat4("projection_matrix", projection_matrix);

	return;
}

void Renderer::render()
{
	// set background to be red
	glClearColor(0.92f, 0.28f, 0.37f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw triangle
	defaultShader.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	return;
}

void Renderer::updateViewMatrix(int new_width, int new_height)
{
	// update projection matrix so that 0,0 is in the center of the window
	projection_matrix = glm::ortho((float)new_width / -2, (float)new_width / 2, (float)new_height / -2, (float)new_height / 2, -100.0f, 100.0f);
	defaultShader.setMat4("projection_matrix", projection_matrix);

	return;
}