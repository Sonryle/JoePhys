#ifndef SHAPES_H
#define SHAPES_H

// main header file
#include <main.hpp>

// vector & matrices
#include <glm/glm.hpp>

class Circle
{
public:

	unsigned int ID;
	float radius;
	glm::vec2 position;

	Circle();
};

class Line
{
public:

	unsigned int ID;
	float thickness;
	glm::vec2 start_position;
	glm::vec2 end_position;

	Line();
};

class Square
{
public:

	unsigned int ID;
	float x_scale;
	float y_scale;
	glm::vec2 position;
	glm::vec2 top_left_tex_coord;
	glm::vec2 top_right_tex_coord;
	glm::vec2 bottom_left_tex_coord;
	glm::vec2 bottom_right_tex_coord;

	Square();
};

#endif