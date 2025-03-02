#ifndef JP_COLOURS
#define JP_COLOURS

#include <cmath>
#include <cfloat> 	// for FLT_MAX
#include <cstdio>	// for "stderr" file path constant

typedef float real;

struct colour
{
	colour() {}
	colour(real r_in, real g_in, real b_in, real a_in) : r(r_in), g(g_in), b(b_in), a(a_in) {}
	void Set(real r_in, real g_in, real b_in, real a_in) {
		r = r_in; g = g_in; b = b_in; a = a_in;
	}
	float r, g, b, a;
};

#endif
