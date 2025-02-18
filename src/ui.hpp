#ifndef UI_H
#define UI_H

// vector & matrices
#include <glm/glm.hpp>
#include <string>

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
	std::string alignment = "topRight";

	UI();

	void updateUI(glm::vec2 window)
	{
		if (alignment == "topRight")
		{
			position = glm::vec2((window.x / 2.0f) - (x_scale / 2.0f) - offset, (window.y / 2.0f) - (y_scale / 2.0f) - offset);
		}
		if (alignment == "topLeft")
		{
			position = glm::vec2(-(window.x / 2.0f) + (x_scale / 2.0f) + offset, (window.y / 2.0f) - (y_scale / 2.0f) - offset);
		}
		if (alignment == "bottomLeft")
		{
			position = glm::vec2(-(window.x / 2.0f) + (x_scale / 2.0f) + offset, -(window.y / 2.0f) + (y_scale / 2.0f) + offset);
		}
		if (alignment == "bottomRight")
		{
			position = glm::vec2((window.x / 2.0f) - (x_scale / 2.0f) - offset, -(window.y / 2.0f) + (y_scale / 2.0f) + offset);
		}
		if (alignment == "centered")
		{
			position = glm::vec2(0, 0);
		}
	}
};

#endif
