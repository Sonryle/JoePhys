#include "shapes.hpp"

Circle::Circle() :
position_in_stack(-1),
layer(1),
radius(50),
position(0),
colour(0)
{
	return;
}

Line::Line() :
position_in_stack(-1),
layer(1),
thickness(10),
start_position(0),
end_position(100),
colour(0)
{
	return;
}

Square::Square() :
position_in_stack(-1),
layer(1),
x_scale(50),
y_scale(50),
position(0),
top_left_tex_coord(0),
top_right_tex_coord(0),
bottom_left_tex_coord(0),
bottom_right_tex_coord(0),
colour(0)
{
	return;
}
