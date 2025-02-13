#include "shapes.hpp"

Circle::Circle() : radius(50), ID(-1)
{
	std::cout << "circle constructor called!\n";
	ID = addCircle(reinterpret_cast<int*>(this));
	return;
}

Line::Line() : thickness(50), ID(-1)
{
	ID = addLine(reinterpret_cast<int*>(this));
	return;
}

Square::Square() : x_scale(50), y_scale(50), ID(-1)
{
	ID = addSquare(reinterpret_cast<int*>(this));
	return;
}