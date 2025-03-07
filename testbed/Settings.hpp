#ifndef JP_SETTINGS
#define JP_SETTINGS

#include "Colour.hpp"

struct SceneColours
{
	unsigned int background;
	unsigned int circles;
	unsigned int circle_outlines;
};

struct Settings
{

	Settings()
	{
		Reset();
	}

	void Reset()
	{
		ResetSceneColours();

		initial_window_width = 1500;
		initial_window_height = 900;

		circle_res = 32;
	}

	void ResetSceneColours()
	{
		scene_colours.background = Palette::JP_DARK_GRAY;
		scene_colours.circles = Palette::JP_RED;
		scene_colours.circle_outlines = Palette::JP_DARK_RED;
	}

	// Colours
	SceneColours scene_colours;

	// Window
	unsigned int initial_window_width;
	unsigned int initial_window_height;

	// Renderer
	int circle_res;

	// Scenes
	bool scene_has_changed;
	unsigned int scene_number;
};

extern Settings settings;

#endif
