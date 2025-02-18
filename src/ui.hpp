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
	float bezelThickness;
	glm::vec2 position;
	glm::vec4 colour;
	glm::vec4 bezelColour;

	UI();

	void update(glm::vec2 window)
	{
		position = glm::vec2((window.x / 2.0f) - (x_scale / 2.0f), (window.y / 2.0f) - (y_scale / 2.0f));
	}
};

#endif
