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
		initial_window_width = 1500;
		initial_window_height = 900;

		scene_colours.background = &palette.dark_gray;
	}

	SceneColours scene_colours;
	unsigned int initial_window_width;
	unsigned int initial_window_height;
};

extern Settings settings;

#endif
