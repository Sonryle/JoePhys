#ifndef SPRITE_H
#define SPRITE_H

#include <iostream>
#include <glm/glm.hpp>

class Sprite
{
public:
	
	// positions and size (position starts from the top left of the sprite)
	glm::vec2 position;
	float width, height;

	// texture coordinates
	glm::vec2 top_left_tex_coord;
	glm::vec2 top_right_tex_coord;
	glm::vec2 bottom_left_tex_coord;
	glm::vec2 bottom_right_tex_coord;
	
};

#endif