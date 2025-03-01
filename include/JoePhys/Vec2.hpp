#ifndef JP_VECTORS
#define JP_VECTORS

/* #include <cmath> */
/* #include <cfloat> 	// for FLT_MAX */
/* #include <cstdio>	// for "stderr" file path constant */

/* typedef float real; */

/* struct vec2 */
/* { */
/* 	// CONSTRUCTORS */
/* 	// ------------ */
	
/* 	// default constructor */
/* 	vec2() : x(0), y(0) {} */
/* 	// constructor using two floats for x & y */
/* 	vec2(real x_in, real y_in) : x(x_in), y(y_in) {} */
/* 	// constructor using another vec2 */
/* 	vec2(const vec2& v) : x(v.x), y(v.y) {} */
/* 	// constructor to create a vector in a given direction */
/* 	vec2(real dir) : x(std::cos(dir)), y(std::sin(dir)) {} */

/* 	// FUNCTIONS */
/* 	// --------- */
	
/* 	void set(const real xIn, const real yIn) { x = xIn; y = yIn; } */
/* 	// ----------------------------------------------- */
/* 	void set(const vec2& o) { x = o.x; y = o.x; } */
/* 	// ----------------------------------------------- */
/* 	void reset() { x = 0; y = 0; } */
/* 	// ----------------------------------------------- */
/* 	void reverse() { x = -x; y = -y; } */
/* 	// ----------------------------------------------- */
/* 	vec2 normal() const { return vec2(-y, x); } */
/* 	// ----------------------------------------------- */
/* 	real len() const { return sqrt(x * x + y * y); } */
/* 	// ----------------------------------------------- */
/* 	real lengthSqr() const { return x * x + y * y; } */
/* 	// ----------------------------------------------- */
/* 	bool isValid() const { return std::isfinite(x) && std::isfinite(y); } */
/* 	// ----------------------------------------------- */
/* 	vec2 normalize() */
/* 	{ */
/* 		real length = len(); */
/* 		if (length < EPSILON) */
/* 			return vec2(0, 0); */
/* 		x /= length; */
/* 		y /= length; */

/* 		return *this; */
/* 	} */

/* 	// OPERATOR FUNCTIONS */
/* 	// ------------------ */

/* 	// With Other Vec2's */

/* 	vec2 operator - () const { return vec2(-x, -y); } */

/* 	bool operator == (const vec2& o) const { return x == o.x && y == o.y; } */
/* 	bool operator != (const vec2& o) const { return x != o.x || y != o.y; } */

/* 	vec2& operator = (const vec2& o) { x = o.x; y = o.y; return *this; } */
/* 	vec2& operator += (const vec2& o) { x += o.x; y += o.y; return *this; } */
/* 	vec2& operator -= (const vec2& o) { x -= o.x; y -= o.y; return *this; } */
/* 	vec2& operator *= (const vec2& o) { x *= o.x; y *= o.y; return *this; } */
/* 	vec2& operator /= (const vec2& o) */
/* 	{ */
/* 		if (std::abs(o.x) < EPSILON || std::abs(o.y) < EPSILON) */
/* 			fprintf(stderr, "Vec2: Division by 0 has occurred\n"); */
/* 		x /= o.x; y /= o.y; */
/* 		return *this; */
/* 	} */

/* 	vec2 operator + (const vec2& o) const {	return vec2(x + o.x, y + o.y); } */
/* 	vec2 operator - (const vec2& o) const { return vec2(x - o.x, y - o.y); } */
/* 	vec2 operator * (const vec2& o) const { return vec2(x * o.x, y * o.y); } */
/* 	vec2 operator / (const vec2& o) const */
/* 	{ */
/* 		if (std::abs(o.x) < EPSILON || std::abs(o.y) < EPSILON) */
/* 			fprintf(stderr, "Vec2: Division by 0 had occurred\n"); */
/* 		return vec2(x / o.x, y / o.y); */
/* 	} */
	
/* 	// With Scalars (real) */
	
/* 	vec2& operator += (const real o) { x += o; y += o; return *this; } */
/* 	vec2& operator -= (const real o) { x -= o; y -= o; return *this; } */
/* 	vec2& operator *= (const real o) { x *= o; y *= o; return *this; } */
/* 	vec2& operator /= (const real o) */
/* 	{ */
/* 		if (std::abs(o) < EPSILON) */
/* 			fprintf(stderr, "Vec2: Division by 0 has occurred\n"); */
/* 		x /= o; y /= o; */
/* 		return *this; */
/* 	} */

/* 	vec2 operator + (const real o) const { return vec2(x + o, y + o); } */
/* 	vec2 operator - (const real o) const { return vec2(x - o, y - o); } */
/* 	vec2 operator * (const real o) const { return vec2(x * o, y * o); } */
/* 	vec2 operator / (const real o) const */
/* 	{ */
/* 		if (std::abs(o) < EPSILON) */
/* 			fprintf(stderr, "Vec2: Division by 0 has occurred\n"); */
/* 		return vec2(x / o, y / o); */
/* 	} */

/* 	// VARIABLES */
/* 	// --------- */

/* 	real x, y; */

/* private: */

/* 	//real constants for pi/epsilon/max. */
/* 	const real EPSILON = 0.0001f; */
/* 	const real PI = 3.14159265359f; */
/* 	const real maxf = FLT_MAX; */

/* }; */

/* inline real crossProduct(const vec2& a, const vec2& b) */
/* { */
/* 	return (a.x * b.y) - (a.y * b.x); */
/* } */

/* inline real dotProduct(const vec2& a, const vec2& b) */
/* { */
/* 	return (a.x * b.x) + (a.y * b.y); */
/* } */

/* inline real distance(const vec2& a, const vec2& b) */
/* { */
/* 	return (a - b).len(); */
/* } */

#pragma once

#include <cfloat>
#include <cmath>

typedef float real;

//real constants for pi/epsilon/max.
const real EPSILON = 0.0001f;
const real PI = 3.14159265359f;
const real maxf = FLT_MAX;

//Checks to make sure that a number is not NaN or infinity.
inline bool isValidCheck(real x) {
	return std::isfinite(x);
}

struct vec2
{
	//Default constructor.
	vec2() {}

	// Constructor using two floats to initialise x and y.
	vec2(real xIn, real yIn) : x(xIn), y(yIn) {}

	// Constructor to construct a normalized vector in a given direction.
	vec2(real dir)
	{
		x = std::cos(dir);
		y = std::sin(dir);
	}

	// Copy constructor using two floats to initialise x and y.
	vec2(const vec2& v) : x(v.x), y(v.y) {}

	// Sets this vector to a given set of x and y values.
	void set(real xIn, real yIn) { x = xIn; y = yIn; }

	// Sets this vector to another vectors given values.
	void set(const vec2& v)
	{
		x = v.x; y = v.y;
	}

	// Resets the vectors values to zero.
	void setZero() { x = 0.0; y = 0.0; }

	// Checks to see if the vectors x and y values are real real values.
	bool isValid() const { return isValidCheck(x) && isValidCheck(y); }

	// Make the given vector negative.
	vec2 operator -() const { return vec2(-x, -y); }

	// Make this vector negative
	void negative() { x = -x; y = -y;}

	// Return a negative vector of this
	vec2 negativeVec() { return vec2(-x, -y); }

	// Add a vector to this vector.
	void operator+=(const vec2& v)
	{
		x += v.x;
		y += v.y;
	}

	// Multiply this vector by a scalar.
	void operator*=(real s)
	{
		x *= s;
		y *= s;
	}

	// Subtract a vector from this vector.
	void operator-=(const vec2& v)
	{
		x -= v.x;
		y -= v.y;
	}

	// Returns the length of this vector
	real len() const
	{
		return sqrt(x * x + y * y);
	}

	// Returns the length squared of this vector
	real lengthSqr() const
	{
		return x * x + y * y;
	}

	// Normalizes this vector and returns the unit vector.
	real Normalize()
	{
		real length = len();
		if (length < EPSILON)
		{
			return 1.0f;
		}
		x /= length;
		y /= length;

		return length;
	}

	// Returns a normalized vector
	vec2 normalizeVec() const
	{
		real d = len();

		if (d == 0.0f) {
			d = 1.0f;
		}
		return vec2(x / d, y / d);
	}

	vec2 normal() const
	{
		return vec2(-y, x);
	}

	real x = 0.0f, y = 0.0f;
};

// Multiply this vector by a scalar value.
inline vec2 operator*(real s, const vec2& v)
{
	return vec2(s * v.x, s * v.y);
}

// Multiply this vector by a scalar value.
inline vec2 operator*(const vec2& v, real s)
{
	return vec2(s * v.x, s * v.y);
}

// Add two vectors together.
inline vec2 operator + (const vec2& a, const vec2& b)
{
	return vec2(a.x + b.x, a.y + b.y);
}

// Subtract a vector from another.
inline vec2 operator - (const vec2& a, const vec2& b)
{
	return vec2(a.x - b.x, a.y - b.y);
}

// Equal to by value.
inline bool operator == (const vec2& a, const vec2& b)
{
	return a.x == b.x && a.y == b.y;
}

// Not equal to by value.
inline bool operator != (const vec2& a, const vec2& b)
{
	return a.x != b.x || a.y != b.y;
}

// Cross product of a vector and a scalar in 2D space. Beware uses left hand rule (normal are a right angle on the left side of the vector passed)
inline vec2 crossProduct(const vec2& v, real a)
{
	return v.normal() * a;
}

// Cross product of a vector and a scalar in 2D space.
inline vec2 crossProduct(real a, const vec2& v)
{
	return crossProduct(v, a);
}

// Cross product of two vectors in 2D space.
inline real crossProduct(const vec2& a, const vec2& b)
{
	return (a.x * b.y) - (a.y * b.x);
}

// Dot product of two vectors.
inline real dotProduct(const vec2& a, const vec2& b) {
	return a.x * b.x + a.y * b.y;
}

// returns the distance because two vectors as a unit vector.
inline real distance(const vec2& a, const vec2& b) {
	return (a - b).len();
}

#endif
