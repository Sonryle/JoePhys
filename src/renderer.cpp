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
	line_shader.init("res/shaders/line_shader.vert", "res/shaders/line_shader.frag");
	square_shader.init("res/shaders/square_shader.vert", "res/shaders/square_shader.frag");
	ui_shader.init("res/shaders/ui_shader.vert", "res/shaders/ui_shader.frag");

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
	circle_shader.use();
	circle_shader.setMat4("projection_matrix", projection_matrix);
	line_shader.use();
	line_shader.setMat4("projection_matrix", projection_matrix);
	square_shader.use();
	square_shader.setMat4("projection_matrix", projection_matrix);
	ui_shader.use();
	ui_shader.setMat4("projection_matrix", projection_matrix);

	return;
}

void Renderer::updateViewMatrix(int new_width, int new_height)
{
	// update projection matrix so that 0,0 is in the center of the window
	projection_matrix = glm::ortho((float)new_width / -2, (float)new_width / 2, (float)new_height / -2, (float)new_height / 2, -100.0f, 100.0f);

	// send new projection matrix to shaders
	circle_shader.use();
	circle_shader.setMat4("projection_matrix", projection_matrix);
	line_shader.use();
	line_shader.setMat4("projection_matrix", projection_matrix);
	square_shader.use();
	square_shader.setMat4("projection_matrix", projection_matrix);
	ui_shader.use();
	ui_shader.setMat4("projection_matrix", projection_matrix);

	return;
}

void Renderer::renderCircle(Circle* circle)
{	
	circle_shader.use();

	circle_shader.setInt("layer", circle->layer);
	circle_shader.setFloat("radius", circle->radius);
	circle_shader.setVec2("position", circle->position);
	circle_shader.setVec4("colour", circle->colour);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::renderLine(Line* line)
{
	line_shader.use();

	line_shader.setVec2("start_position", line->start_position);
	line_shader.setVec2("end_position", line->end_position);
	line_shader.setFloat("thickness", line->thickness);
	line_shader.setVec4("colour", line->colour);
	line_shader.setInt("layer", line->layer);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}


void Renderer::renderSquare(Square* square)
{
	square_shader.use();

	square_shader.setVec4("colour", square->colour);
	square_shader.setInt("layer", square->layer);
	square_shader.setFloat("x_scale", square->x_scale);
	square_shader.setFloat("y_scale", square->y_scale);
	square_shader.setVec2("position", square->position);
	square_shader.setVec2("top_left_tex_coord", square->top_left_tex_coord);
	square_shader.setVec2("top_right_tex_coord", square->top_right_tex_coord);
	square_shader.setVec2("bottom_left_tex_coord", square->bottom_left_tex_coord);
	square_shader.setVec2("bottom_right_tex_coord", square->bottom_right_tex_coord);

	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Renderer::renderUI(UI* ui)
{
	ui_shader.use();

	ui_shader.setVec4("colour", ui->colour);
	ui_shader.setInt("layer", ui->layer);
	ui_shader.setFloat("x_scale", ui->x_scale);
	ui_shader.setFloat("y_scale", ui->y_scale);
	ui_shader.setFloat("offset", ui->offset);
	ui_shader.setVec2("position", ui->position);
	ui_shader.setVec2("top_left_tex_coord", ui->top_left_tex_coord);
	ui_shader.setVec2("top_right_tex_coord", ui->top_right_tex_coord);
	ui_shader.setVec2("bottom_left_tex_coord", ui->bottom_left_tex_coord);
	ui_shader.setVec2("bottom_right_tex_coord", ui->bottom_right_tex_coord);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}
