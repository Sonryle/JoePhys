#ifndef JP_COLOURS
#define JP_COLOURS

#include <cmath>
#include <cfloat> 	// for FLT_MAX
#include <cstdio>	// for "stderr" file path constant

typedef float real;

/* struct colour */
/* { */
/* 	// CONSTRUCTORS */
/* 	// ------------ */
	
/* 	// defualt constructor */
/* 	colour() */
/* 	{ */
/* 		r = 0.0f; */
/* 		g = 0.0f; */
/* 		b = 0.0f; */
/* 		a = 0.0f; */
/* 	}; */
/* 	// constructor using four floats for r, g, b and a */
/* 	colour(real r_in, real g_in, real b_in, real a_in) */
/* 	{ */
/* 		r = r_in; */
/* 		g = g_in; */
/* 		b = b_in; */
/* 		a = a_in; */
/* 	}; */

	// FUNCTIONS
	// ---------
	
	/* void set(real r_in, real g_in, real b_in, real a_in) */
	/* { */
	/* 	r = r_in; */
	/* 	g = g_in; */
	/* 	b = b_in; */
	/* 	a = a_in; */
	/* }; */

	// OPERATOR FUNCTIONS
	// ------------------
	
	// With Other Colours

/* 	colour operator - () const { return colour(-r, -g, -b, -a); } */

/* 	bool operator != (const colour& o) const */
/* 	{ */
/* 		 return !( */
/* 		 	r == o.r && */
/* 			g == o.g && */
/* 			b == o.b && */
/* 			a == o.a */
/* 			); */
/* 	} */
/* 	bool operator == (const colour& o) const */
/* 	{ */
/* 		 return r == o.r && */
/* 			g == o.g && */
/* 			b == o.b && */
/* 			a == o.a; */
/* 	} */

/* 	colour operator = (const colour& o) */
/* 	{ */
/* 		r = o.r; */
/* 		g = o.g; */
/* 		b = o.b; */
/* 		a = o.a; */
/* 		return *this; */
/* 	} */
/* 	colour operator += (const colour& o) */
/* 	{ */
/* 		r += o.r; */
/* 		g += o.g; */
/* 		b += o.b; */
/* 		a += o.a; */
/* 		return *this; */
/* 	} */
/* 	colour operator -= (const colour& o) */
/* 	{ */
/* 		r -= o.r; */
/* 		g -= o.g; */
/* 		b -= o.b; */
/* 		a -= o.a; */
/* 		return *this; */
/* 	} */
/* 	colour operator *= (const colour& o) */
/* 	{ */
/* 		r *= o.r; */
/* 		g *= o.g; */
/* 		b *= o.b; */
/* 		a *= o.a; */
/* 		return *this; */
/* 	} */
/* 	colour operator /= (const colour& o) */
/* 	{ */
/* 		bool illegal =  std::abs(o.r) < EPSILON || */
/* 				std::abs(o.g) < EPSILON || */
/* 				std::abs(o.b) < EPSILON || */
/* 				std::abs(o.a) < EPSILON; */
/* 		if (illegal) */
/* 			fprintf(stderr, "Colour: Division by 0 has occurred\n"); */
/* 		r /= o.r; */
/* 		g /= o.g; */
/* 		b /= o.b; */
/* 		a /= o.a; */
/* 		return *this; */
/* 	} */
/* 	colour operator + (const colour& o) const */
/* 	{ */
/* 		return colour(r+o.r, g+o.g, b+o.b, a+o.a); */
/* 	} */
/* 	colour operator - (const colour& o) const */
/* 	{ */
/* 		return colour(r-o.r, g-o.g, b-o.b, a-o.a); */
/* 	} */
/* 	colour operator * (const colour& o) const */
/* 	{ */
/* 		return colour(r*o.r, g*o.g, b*o.b, a*o.a); */
/* 	} */
/* 	colour operator / (const colour& o) const */
/* 	{ */
/* 		bool illegal =  std::abs(o.r) < EPSILON || */
/* 				std::abs(o.g) < EPSILON || */
/* 				std::abs(o.b) < EPSILON || */
/* 				std::abs(o.a) < EPSILON; */
/* 		if (illegal) */
/* 			fprintf(stderr, "Colour: Division by 0 has occurred\n"); */
/* 		return colour(r/o.r, g/o.g, b/o.b, a/o.a); */
/* 	} */

/* 	// With Scalars (real) */

/* 	colour& operator = (const real o) */
/* 	{ */
/* 		r = o; */
/* 		g = o; */
/* 		b = o; */
/* 		a = o; */
/* 		return *this; */
/* 	} */
/* 	colour& operator += (const real o) */
/* 	{ */
/* 		r += o; */
/* 		g += o; */
/* 		b += o; */
/* 		a += o; */
/* 		return *this; */
/* 	} */
/* 	colour& operator -= (const real o) */
/* 	{ */
/* 		r -= o; */
/* 		g -= o; */
/* 		b -= o; */
/* 		a -= o; */
/* 		return *this; */
/* 	} */
/* 	colour& operator *= (const real o) */
/* 	{ */
/* 		r *= o; */
/* 		g *= o; */
/* 		b *= o; */
/* 		a *= o; */
/* 		return *this; */
/* 	} */
/* 	colour& operator /= (const real o) */
/* 	{ */
/* 		if (std::abs(o) < EPSILON) */
/* 			fprintf(stderr, "Colour: Division by 0 has occurred\n"); */
/* 		r /= o; */
/* 		g /= o; */
/* 		b /= o; */
/* 		a /= o; */
/* 		return *this; */
/* 	} */
/* 	colour operator + (const real o) const */
/* 	{ */
/* 		return colour(r+o, g+o, b+o, a+o); */
/* 	} */
/* 	colour operator - (const real o) const */
/* 	{ */
/* 		return colour(r-o, g-o, b-o, a-o); */
/* 	} */
/* 	colour operator * (const real o) const */
/* 	{ */
/* 		return colour(r*o, g*o, b*o, a*o); */
/* 	} */
/* 	colour operator / (const real o) const */
/* 	{ */
/* 		if (std::abs(o) < EPSILON) */
/* 			fprintf(stderr, "Colour: Division by 0 has occurred\n"); */
/* 		return colour(r/o, g/o, b/o, a/o); */
/* 	} */

	// VARIABLES
	// ---------

	/* real r, g, b, a; */

/* private: */

	/* //real constants for pi/epsilon/max. */
	/* /1* const real EPSILON = 0.0001f; *1/ */
	/* /1* const real PI = 3.14159265359f; *1/ */
	/* /1* const real maxf = FLT_MAX; *1/ */
	
/* }; */

/* #pragma once */

struct colour
{
	colour() {}
	colour(float rIn, float gIn, float bIn, float aIn) : r(rIn), g(gIn), b(bIn), a(aIn) {}
	void Set(float rIn, float gIn, float bIn, float aIn) {
		r = rIn; g = gIn; b = bIn; a = aIn;
	}
	float r, g, b, a;
};

#endif
