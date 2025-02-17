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
	float bezel;
	glm::vec2 position;
	glm::vec4 colour;
	glm::vec4 bezelColour;

	UI();
};

#endif
