#ifndef SHAPES_H
#define SHAPES_H

// vector & matrices
#include <glm/glm.hpp>

class Circle
{
public:

	int layer;
	float radius;
	glm::vec2 position;
	glm::vec4 colour;

	Circle();
};

class Line
{
public:

	int layer;
	float thickness;
	glm::vec2 start_position;
	glm::vec2 end_position;
	glm::vec4 colour;

	Line();
};

class Square
{
public:

	int layer;
	float x_scale;
	float y_scale;
	glm::vec2 position;
	glm::vec2 top_left_tex_coord;
	glm::vec2 top_right_tex_coord;
	glm::vec2 bottom_left_tex_coord;
	glm::vec2 bottom_right_tex_coord;
	glm::vec4 colour;

	Square();
};

#endif
