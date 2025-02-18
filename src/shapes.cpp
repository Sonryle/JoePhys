#include "shapes.hpp"

Circle::Circle() :
layer(1),
radius(50),
position(0),
colour(0)
{
	return;
}

Line::Line() :
layer(1),
thickness(10),
start_position(0),
end_position(100),
colour(0)
{
	return;
}

Square::Square() :
layer(1),
x_scale(50),
y_scale(50),
position(0),
top_left_tex_coord(0),
top_right_tex_coord(0),
bottom_left_tex_coord(0),
bottom_right_tex_coord(0)
{
	colour = glm::vec4(-1.0f, -1.0f, -1.0f, -1.0f);
	return;
}
