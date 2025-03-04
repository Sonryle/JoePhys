#ifndef JP_CIRCLE
#define JP_CIRCLE

#include "Shape.hpp"

class Circle : public Shape
{
public:
	Circle(real radius) : radius(radius) {}

	Type GetType() const override { return eCircle; }
	real radius = 0.0f;
};

#endif
