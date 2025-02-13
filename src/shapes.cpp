#include "shapes.hpp"

Circle::Circle() : radius(50)
{
	renderer.circle_stack.push_back(this);
	ID = (int)renderer.circle_stack.size() - 1;

	return;
}

Line::Line() : thickness(50)
{
	renderer.line_stack.push_back(this);
	ID = (int)renderer.line_stack.size() - 1;

	return;
}

Square::Square() : x_scale(50), y_scale(50)
{
	renderer.square_stack.push_back(this);
	ID = (int)renderer.square_stack.size() - 1;

	return;
}