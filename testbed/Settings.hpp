#ifndef JP_SETTINGS
#define JP_SETTINGS

#include "Colours.hpp"

struct SceneColours
{
	colour* background;
	colour* triangles;
	colour* lines;
};

struct Settings
{

	Settings()
	{
		Reset();
	}

	void Reset()
	{
		scene_colours.background = &palette.dark_gray;

		initial_window_width = 1500;
		initial_window_height = 900;

		circle_segments = 10;
	}

	// Colours
	SceneColours scene_colours;

	// Window
	unsigned int initial_window_width;
	unsigned int initial_window_height;

	// Renderer
	int circle_segments;
};

extern Settings settings;

#endif
