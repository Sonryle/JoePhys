#include "renderer.hpp"

float square_vertex[] = {
	// positions		// texture coordinates
	 1.0f,  1.0f, -1.0f,	1.0f, 1.0f,	// top right
	 1.0f, -1.0f, -1.0f,	1.0f, 0.0f,	// bottom right
	-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,	// top left

	// positions		// texture coordinates
	 1.0f, -1.0f, -1.0f,	1.0f, 0.0f,	// bottom right
	-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,	// bottom left
	-1.0f,  1.0f, -1.0f,	0.0f, 1.0f	// top left
};

float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner  
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
};

void Renderer::init(int window_width, int window_height)
{
	std::cout << "initialising renderer!\n";

	// OPENGL SETUP
	// ------------
	
	// set gl viewport to be the same resolution as our window
	glViewport(0, 0, window_width, window_height);

	// enable depth testing and transparency
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	// tell openGL how depth testing & transparency should behave
	glDepthFunc(GL_LEQUAL);	// only render if fragments depth >= than depth buffer
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set texture wrapping options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// set texture interpolation options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// TEXTURE ATLAS SETUP
	// -------------------
	
	// make texture atlas the right way up
	stbi_set_flip_vertically_on_load(true);

	// load texture_atlas.png using stb
	int width, height, nrChannels;
	unsigned char *data = stbi_load("res/textures/texture_atlas.png", &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
    		std::cout << "Failed to load texture" << std::endl;
	}

	// generate an openGL texture
	glGenTextures(1, &texture_atlas);

	// bind our texture so openGL knows to use it
	glBindTexture(GL_TEXTURE_2D, texture_atlas);

	// attach our loaded texture data to our texture object
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// free the image memory since it's now stored in our texture object
	stbi_image_free(data);

	// SHADER SETUP
	// ------------

	// initialise shaders & give shader paths
	circle_shader.init("res/shaders/circle_shader.vert", "res/shaders/circle_shader.frag");
	line_shader.init("res/shaders/line_shader.vert", "res/shaders/line_shader.frag");
	square_shader.init("res/shaders/square_shader.vert", "res/shaders/square_shader.frag");

	// Update view matrix
	// ------------------
	
	updateViewMatrix(window_width, window_height);

	// VERTEX DATA SETUP
	// -----------------

	// Set up VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Set up VBO and attach buffer data
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertex), square_vertex, GL_STATIC_DRAW);

	// Set vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
