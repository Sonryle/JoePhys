#ifndef JP_COLOURS
#define JP_COLOURS

#include <cmath>
#include <cfloat> 	// for FLT_MAX
#include <cstdio>	// for "stderr" file path constant

typedef float real;

struct colour
{
	colour() {}
	colour(real rIn, real gIn, real bIn, real aIn) : r(rIn), g(gIn), b(bIn), a(aIn) {}
	void Set(real rIn, real gIn, real bIn, real aIn) {
		r = rIn; g = gIn; b = bIn; a = aIn;
	}
	float r, g, b, a;
};

#endif
