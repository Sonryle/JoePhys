#include "renderer.hpp"

float square_vertex[] = {
	// first triangle
	 1.0f,  1.0f, -1.0f,  // top right
	 1.0f, -1.0f, -1.0f,  // bottom right
	-1.0f,  1.0f, -1.0f,  // top left
	// second triangle
	 1.0f, -1.0f, -1.0f,  // bottom right
	-1.0f, -1.0f, -1.0f,  // bottom left
	-1.0f,  1.0f , -1.0f   // top left
};

void Renderer::init()
{
	std::cout << "initialising renderer!\n";

	// Set up shaders

	circle_shader.init("res/shaders/circle_shader.vert", "res/shaders/circle_shader.frag");
	circle_shader.use();

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
	circle_shader.setMat4("projection_matrix", projection_matrix);

	return;
}

void Renderer::render()
{
	// set background to be red
	glClearColor(0.92f, 0.28f, 0.37f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw our shapes
	glBindVertexArray(VAO);

	// loop over all circles and render
	circle_shader.use();
	for (int i = 0; i < (int)circle_stack.size(); i++)
	{
		Circle* current_circle = reinterpret_cast<Circle*>(circle_stack[i]);

		circle_shader.setVec2("position", current_circle->position);
		circle_shader.setFloat("radius", current_circle->radius);
		circle_shader.setVec4("colour", current_circle->colour);
		circle_shader.setInt("layer", current_circle->layer);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	return;
}

void Renderer::updateViewMatrix(int new_width, int new_height)
{
	// update projection matrix so that 0,0 is in the center of the window
	projection_matrix = glm::ortho((float)new_width / -2, (float)new_width / 2, (float)new_height / -2, (float)new_height / 2, -100.0f, 100.0f);
	circle_shader.setMat4("projection_matrix", projection_matrix);

	return;
}