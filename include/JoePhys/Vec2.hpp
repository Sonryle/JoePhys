#ifndef JP_VECTORS
#define JP_VECTORS

#include <cmath>	// for sin & cos
#include <cfloat> 	// for FLT_MAX
#include <cstdio>	// for "stderr" file path constant
#include <stdexcept>	// for "runtime_error"

typedef float real;

//real constants for pi/epsilon/max.
const real EPSILON = 0.0001f;
const real PI = 3.14159265359f;
const real maxf = FLT_MAX;

struct vec2
{
	// CONSTRUCTORS
	// ------------
	
	// default constructor
	vec2() : x(0), y(0) {}
	// constructor using two floats for x & y
	vec2(real xIn, real yIn) : x(xIn), y(yIn) {}
	// constructor using another vec2
	vec2(const vec2& v) : x(v.x), y(v.y) {}
	// constructor to create a vector in a given direction
	vec2(real dir) : x(std::cos(dir)), y(std::sin(dir)) {}

	// FUNCTIONS
	// ---------
	
	void set(const real xIn, const real yIn) { x = xIn; y = yIn; }
	// -----------------------------------------------
	void set(const vec2& o) { x = o.x; y = o.x; }
	// -----------------------------------------------
	void reset() { x = 0; y = 0; }
	// -----------------------------------------------
	void reverse() { x = -x; y = -y; }
	// -----------------------------------------------
	vec2 normal() const { return vec2(-y, x); }
	// -----------------------------------------------
	real len() const { return sqrt(x * x + y * y); }
	// -----------------------------------------------
	real lengthSqr() const { return x * x + y * y; }
	// -----------------------------------------------
	bool isValid() const { return std::isfinite(x) && std::isfinite(y); }
	// -----------------------------------------------
	vec2 normalize()
	{
		real length = len();
		if (length < EPSILON)
			return vec2(0, 0);
		x /= length;
		y /= length;

		return *this;
	}

	// OPERATOR FUNCTIONS
	// ------------------

	// With Other Vec2's

	vec2 operator - () const { return vec2(-x, -y); }

	bool operator == (const vec2& o) const { return x == o.x && y == o.y; }
	bool operator != (const vec2& o) const { return x != o.x || y != o.y; }

	vec2& operator += (const vec2& o) { x += o.x; y += o.y; return *this; }
	vec2& operator -= (const vec2& o) { x -= o.x; y -= o.y; return *this; }
	vec2& operator *= (const vec2& o) { x *= o.x; y *= o.y; return *this; }
	vec2& operator /= (const vec2& o) { x /= o.x; y /= o.y; return *this; }

	vec2 operator + (const vec2& o) const {	return vec2(x + o.x, y + o.y); }
	vec2 operator - (const vec2& o) const { return vec2(x - o.x, y - o.y); }
	vec2 operator * (const vec2& o) const { return vec2(x * o.x, y * o.y); }
	vec2 operator / (const vec2& o) const {
		if (std::abs(o.x) < EPSILON || std::abs(o.y) < EPSILON)
			throw std::runtime_error("Vec2: Division by 0 not allowed");
		return vec2(x / o.x, y / o.y);
	}
	
	// With Scalars (real)
	
	vec2& operator += (const real o) { x += o; y += o; return *this; }
	vec2& operator -= (const real o) { x -= o; y -= o; return *this; }
	vec2& operator *= (const real o) { x *= o; y *= o; return *this; }
	vec2& operator /= (const real o) { x /= o; y /= o; return *this; }

	vec2 operator + (const real o) const { return vec2(x + o, y + o); }
	vec2 operator - (const real o) const { return vec2(x - o, y - o); }
	vec2 operator * (const real o) const { return vec2(x * o, y * o); }
	vec2 operator / (const real o) const {
		if (std::abs(o) < EPSILON)
			throw std::runtime_error("Vec2: Division by 0 not allowed");
		return vec2(x / o, y / o);
	}

	// VARIABLES
	// ---------

	real x, y;
};

inline real crossProduct(const vec2& a, const vec2& b)
{
	return (a.x * b.y) - (a.y * b.x);
}

inline real dotProduct(const vec2& a, const vec2& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

inline real distance(const vec2& a, const vec2& b)
{
	return (a - b).len();
}

#endif
