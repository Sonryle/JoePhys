#ifndef JP_SHAPE
#define JP_SHAPE

#include "Mat2.hpp"

class Shape
{
public:
	enum Type
	{
		eCircle, ePolygon
	};

	virtual Type GetType() const = 0;

	void SetRotation(real radians) { rotation.SetRotation(radians); }
	mat2 rotation = mat2(1.0f);
};

#endif
