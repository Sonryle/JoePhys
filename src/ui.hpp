#ifndef UI_H
#define UI_H

// vector & matrices
#include <glm/glm.hpp>

class UI
{
public:

	int position_in_stack;
	int layer;
	float x_scale;
	float y_scale;
    float offset;
	glm::vec2 position;
	glm::vec2 top_left_tex_coord;
	glm::vec2 top_right_tex_coord;
	glm::vec2 bottom_left_tex_coord;
	glm::vec2 bottom_right_tex_coord;
	glm::vec4 colour;

	UI();
};

#endif
