#ifndef JP_COLOUR
#define JP_COLOUR

typedef float real;

// colour struct containing a float for each colour channel, red, green, blue and alpha
struct colour
{
	// constructors
	colour() : r(0), g(0), b(0), a(0) {}
	colour(real r_in, real g_in, real b_in, real a_in) : r(r_in), g(g_in), b(b_in), a(a_in) {}
	colour(colour* col) : r(col->r), g(col->g), b(col->b), a(col->a) {}

	// functions
	void Set(real r_in, real g_in, real b_in, real a_in) { r = r_in; g = g_in; b = b_in; a = a_in; }
	void Set(const colour& col) { Set(col.r, col.g, col.b, col.a); }

	// Conversion operator to float* (returns colour as an array of 3 floats)
	operator const float* () const { return &r; }

	real r, g, b, a;
};

struct Palette
{
	Palette()
	{
		SetAutumn();
	}

	enum COLOUR
	{
		JP_GRAY, JP_RED, JP_GREEN, JP_YELLOW, JP_BLUE, JP_PURPLE, JP_AQUA, JP_WHITE,
		JP_DARK_GRAY, JP_DARK_RED, JP_DARK_GREEN, JP_DARK_YELLOW, JP_DARK_BLUE, JP_DARK_PURPLE,
		JP_DARK_AQUA, JP_DARK_WHITE
	};

	void SetAutumn()
	{
		colours[JP_GRAY].Set(0.16f, 0.16f, 0.16f, 1.0f);
		colours[JP_RED].Set(0.44f, 0.16f, 0.14f, 1.0f);
		colours[JP_GREEN].Set(0.55f, 0.55f, 0.39f, 1.0f);
		colours[JP_YELLOW].Set(0.80f, 0.5f, 0.33f, 1.0f);
		colours[JP_BLUE].Set(0.51f, 0.6f, 0.69f, 1.0f);
		colours[JP_PURPLE].Set(0.58f, 0.48f, 0.65f, 1.0f);
		colours[JP_AQUA].Set(0.13f, 0.44f, 0.44f, 1.0f);
		colours[JP_WHITE].Set(0.67f, 0.67f, 0.67f, 1.0f);

		colours[JP_DARK_GRAY].Set(0.08f, 0.08f, 0.08f, 1.0f);
		colours[JP_DARK_RED].Set(0.29f, 0.11f, 0.12f, 1.0f);
		colours[JP_DARK_GREEN].Set(0.25f, 0.3f, 0.14f, 1.0f);
		colours[JP_DARK_YELLOW].Set(0.55f, 0.34f, 0.23f, 1.0f);
		colours[JP_DARK_BLUE].Set(0.29f, 0.35f, 0.42f, 1.0f);
		colours[JP_DARK_PURPLE].Set(0.36f, 0.2f, 0.37f, 1.0f);
		colours[JP_DARK_AQUA].Set(0.1f, 0.27f, 0.29f, 1.0f);
		colours[JP_DARK_WHITE].Set(0.31f, 0.27f, 0.26f, 1.0f);
	}

	void SetGruvbox()
	{
		colours[JP_GRAY].Set(0.57f, 0.51f, 0.45f, 1.0f);
		colours[JP_RED].Set(0.98f, 0.29f, 0.20f, 1.0f);
		colours[JP_GREEN].Set(0.72f, 0.73f, 0.15f, 1.0f);
		colours[JP_YELLOW].Set(0.98f, 0.74f, 0.18f, 1.0f);
		colours[JP_BLUE].Set(0.51f, 0.65f, 0.6f, 1.0f);
		colours[JP_PURPLE].Set(0.83f, 0.53f, 0.61f, 1.0f);
		colours[JP_AQUA].Set(0.56f, 0.75f, 0.49f, 1.0f);
		colours[JP_WHITE].Set(0.92f, 0.86f, 0.7f, 1.0f);

		colours[JP_DARK_GRAY].Set(0.16f, 0.16f, 0.16f, 1.0f);
		colours[JP_DARK_RED].Set(0.8f, 0.14f, 0.11f, 1.0f);
		colours[JP_DARK_GREEN].Set(0.6f, 0.59f, 0.1f, 1.0f);
		colours[JP_DARK_YELLOW].Set(0.84f, 0.6f, 0.13f, 1.0f);
		colours[JP_DARK_BLUE].Set(0.27f, 0.70f, 0.72f, 1.0f);
		colours[JP_DARK_PURPLE].Set(0.69f, 0.38f, 0.53f, 1.0f);
		colours[JP_DARK_AQUA].Set(0.4f, 0.61f, 0.41f, 1.0f);
		colours[JP_DARK_WHITE].Set(0.66f, 0.6f, 0.52f, 1.0f);
	}

	void SetPastel()
	{
		colours[JP_GRAY].Set(0.52f, 0.52f, 0.52f, 1.0f);
		colours[JP_RED].Set(1.0f, 0.68f, 0.68f, 1.0f);
		colours[JP_GREEN].Set(0.79f, 1.0f, 0.75f, 1.0f);
		colours[JP_YELLOW].Set(0.99f, 1.0f, 0.71f, 1.0f);
		colours[JP_BLUE].Set(0.6f, 0.96f, 1.0f, 1.0f);
		colours[JP_PURPLE].Set(1.0f, 0.78f, 1.0f, 1.0f);
		colours[JP_AQUA].Set(0.59f, 0.95f, 0.82f, 1.0f);
		colours[JP_WHITE].Set(0.89f, 0.89f, 0.89f, 1.0f);

		colours[JP_DARK_GRAY].Set(0.22f, 0.22f, 0.22f, 1.0f);
		colours[JP_DARK_RED].Set(0.7f, 0.38f, 0.38f, 1.0f);
		colours[JP_DARK_GREEN].Set(0.49f, 0.7f, 0.45f, 1.0f);
		colours[JP_DARK_YELLOW].Set(0.69f, 0.7f, 0.41f, 1.0f);
		colours[JP_DARK_BLUE].Set(0.3f, 0.66f, 0.7f, 1.0f);
		colours[JP_DARK_PURPLE].Set(0.7f, 0.48f, 0.7f, 1.0f);
		colours[JP_DARK_AQUA].Set(0.29f, 0.65f, 0.52f, 1.0f);
		colours[JP_DARK_WHITE].Set(0.59f, 0.59f, 0.59f, 1.0f);
	}

	colour colours[16];
};

extern Palette palette;

#endif
