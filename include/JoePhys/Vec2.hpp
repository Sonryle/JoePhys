#ifndef JP_VECTORS
#define JP_VECTORS

// cmath for sqrt
#include <cmath>

typedef float real;

// constant for epsilon
const real EPSILON = 0.0001f;

// 2 dimensional vector (contains x and y value)
struct vec2
{
	// constructors
	vec2() : x(0), y(0) {}
	vec2(real x_in, real y_in) : x(x_in), y(y_in) {}
	vec2(vec2* vec) : x(vec->x), y(vec->y) {}

	// functions
	void Set(real x_in, real y_in) { x = x_in; y = y_in; }
	void Set(vec2* vec) { x = vec->x, y = vec->y; }
	void Set(vec2 vec) { x = vec.x, y = vec.y; }

	real GetLength() { return std::sqrt(x*x + y*y); }
	vec2 GetNormalized() { return vec2(x / GetLength(), y / GetLength()); }

	// operator functions for vec2
	// ---------------------------

	// operations between two vec2 structs
	inline vec2 operator +=(vec2 b) { x += b.x; y += b.y; return this; }
	inline vec2 operator -=(vec2 b) { x -= b.x; y -= b.y; return this; }
	inline vec2 operator *=(vec2 b) { x *= b.x; y *= b.y; return this; }
	inline vec2 operator /=(vec2 b) { x /= b.x; y /= b.y; return this; }
	
	// operations between a vec2 and a float
	inline vec2 operator +=(real b) { x += b; y += b; return this; }
	inline vec2 operator -=(real b) { x -= b; y -= b; return this; }
	inline vec2 operator *=(real b) { x *= b; y *= b; return this; }
	inline vec2 operator /=(real b) { x /= b; y /= b; return this; }

	real x, y;
};

// functions for vec2's
// --------------------

// return length of a vector
inline real length(vec2 v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

// return dot product of two vectors.
inline real dot(const vec2 a, const vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

// return vector that has been normalized
inline vec2 normalize(vec2 a)
{
	return vec2(a.x / length(a), a.y / length(a));
}

// operator functions for vec2
// ---------------------------

// operations between two vec2 structs
inline vec2 operator + (vec2 a, vec2 b) { return vec2(a.x + b.x, a.y + b.y); }
inline vec2 operator - (vec2 a, vec2 b) { return vec2(a.x - b.x, a.y - b.y); }
inline vec2 operator * (vec2 a, vec2 b) { return vec2(a.x * b.x, a.y * b.y); }
inline vec2 operator / (vec2 a, vec2 b) { return vec2(a.x / b.x, a.y / b.y); }
inline bool operator ==(vec2 a, vec2 b) { return a.x == b.x && a.y == b.y; }
inline bool operator !=(vec2 a, vec2 b) { return a.x != b.x || a.y != b.y; }

// return negative vector
inline vec2 operator -(vec2 a) { return vec2(-a.x, -a.y); }

// operations between a vec2 and a float
inline vec2 operator + (vec2 a, real b) { return vec2(a.x + b, a.y + b); }
inline vec2 operator - (vec2 a, real b) { return vec2(a.x - b, a.y - b); }
inline vec2 operator * (vec2 a, real b) { return vec2(a.x * b, a.y * b); }
inline vec2 operator / (vec2 a, real b) { return vec2(a.x / b, a.y / b); }

// operations between a float and a vec2 (other way around)
inline vec2 operator + (real a, vec2 b) { return vec2(b.x + a, b.y + a); }
inline vec2 operator - (real a, vec2 b) { return vec2(b.x - a, b.y - a); }
inline vec2 operator * (real a, vec2 b) { return vec2(b.x * a, b.y * a); }
inline vec2 operator / (real a, vec2 b) { return vec2(b.x / a, b.y / a); }

#endif
