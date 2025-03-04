#ifndef JP_COLOURS
#define JP_COLOURS

#include <cmath>
#include <cfloat> 	// for FLT_MAX
#include <cstdio>	// for "stderr" file path constant

typedef float real;

struct colour;
struct gruv;

struct colour
{
	// CONSTRUCTORS
	// ------------

	colour() {}
	colour(real r_in, real g_in, real b_in, real a_in) : r(r_in), g(g_in), b(b_in), a(a_in) {}

	// FUNCTIONS
	// ---------

	void Set(real r_in, real g_in, real b_in, real a_in) {
		r = r_in; g = g_in; b = b_in; a = a_in;
	}
	void Set(const colour& col) {
		Set(col.r, col.g, col.b, col.a);
	}

	// Conversion operator to float*
	operator float* () { return &r; }
	operator const float* () const { return &r; }

	// VARIABLES
	// ---------

	float r, g, b, a;
};

struct Palette
{
	Palette()
	{
		SetPurpbox();
	}

	void SetGruvbox()
	{
		white.Set(0.92f, 0.86f, 0.7f, 1.0f);
		red.Set(0.98f, 0.29f, 0.20f, 1.0f);
		green.Set(0.72f, 0.73f, 0.15f, 1.0f);
		yellow.Set(0.98f, 0.74f, 0.18f, 1.0f);
		blue.Set(0.51f, 0.65f, 0.6f, 1.0f);
		purple.Set(0.83f, 0.53f, 0.61f, 1.0f);
		aqua.Set(0.56f, 0.75f, 0.49f, 1.0f);
		gray.Set(0.57f, 0.51f, 0.45f, 1.0f);

		dark_white.Set(0.66f, 0.6f, 0.52f, 1.0f);
		dark_red.Set(0.8f, 0.14f, 0.11f, 1.0f);
		dark_green.Set(0.6f, 0.59f, 0.1f, 1.0f);
		dark_yellow.Set(0.84f, 0.6f, 0.13f, 1.0f);
		dark_blue.Set(0.27f, 0.70f, 0.72f, 1.0f);
		dark_purple.Set(0.69f, 0.38f, 0.53f, 1.0f);
		dark_aqua.Set(0.4f, 0.61f, 0.41f, 1.0f);
		dark_gray.Set(0.16f, 0.16f, 0.16f, 1.0f);
	}

	void SetPurpbox()
	{
		white.Set(0.92f, 0.86f, 0.7f, 1.0f);
		red.Set(0.98f, 0.29f, 0.20f, 1.0f);
		green.Set(0.72f, 0.73f, 0.15f, 1.0f);
		yellow.Set(0.98f, 0.74f, 0.18f, 1.0f);
		blue.Set(0.51f, 0.65f, 0.6f, 1.0f);
		purple.Set(0.83f, 0.53f, 0.61f, 1.0f);
		aqua.Set(0.56f, 0.75f, 0.49f, 1.0f);
		gray.Set(0.24f, 0.19f, 0.33f, 1.0f);

		dark_white.Set(0.66f, 0.6f, 0.52f, 1.0f);
		dark_red.Set(0.8f, 0.14f, 0.11f, 1.0f);
		dark_green.Set(0.6f, 0.59f, 0.1f, 1.0f);
		dark_yellow.Set(0.84f, 0.6f, 0.13f, 1.0f);
		dark_blue.Set(0.27f, 0.70f, 0.72f, 1.0f);
		dark_purple.Set(0.69f, 0.38f, 0.53f, 1.0f);
		dark_aqua.Set(0.4f, 0.61f, 0.41f, 1.0f);
		dark_gray.Set(0.17f, 0.13f, 0.23f, 1.0f);
	}

	colour white;
	colour red;
	colour green;
	colour yellow;
	colour blue;
	colour purple;
	colour aqua;
	colour gray;

	colour dark_white;
	colour dark_red;
	colour dark_green;
	colour dark_yellow;
	colour dark_blue;
	colour dark_purple;
	colour dark_aqua;
	colour dark_gray;
};

extern Palette palette;

#endif
