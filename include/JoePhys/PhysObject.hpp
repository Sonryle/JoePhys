#ifndef JP_RIGIDBODY
#define JP_RIGIDBODY

#include "Vec2.hpp"
class Shape;

class PhysObject
{
public:
	PhysObject(Shape* shape, vec2 position, float mass) : shape(shape), position(position), old_position(position), mass(mass) { };

	vec2 position;
	vec2 old_position;
	float mass;

	Shape* shape;
};

#endif
