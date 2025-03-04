#ifndef JP_MAT2
#define JP_MAT2

#include <cmath>
#include "Vec2.hpp"

struct mat2
{
	mat2() {};
	mat2(float value) { row1.x = row2.y = value; }

	void SetRotation(float radians)
	{
		float s = sin(radians);
		float c = cos(radians);

		row1.x = c;
		row1.y = -s;
		row2.x = s;
		row2.y = c;
	}

	const vec2 operator*(const vec2& v) const
	{
		return vec2(row1.x * v.x + row1.y * v.y, row2.x * v.x + row2.y * v.y);
	}

	vec2 row1 = vec2(0.0f, 0.0f);
	vec2 row2 = vec2(0.0f, 0.0f);
};

#endif
