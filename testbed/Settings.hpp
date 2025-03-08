#ifndef JP_SETTINGS
#define JP_SETTINGS

#include "Colour.hpp"

// This struct contains integers which keep track of the colours
// set for everything that gets rendered to the screen. They can
// be assigned colours from the colour palette struct defined in "Colour.hpp"
// (e.g. my_scene_colours_instance.background = Palette::JP_RED)
struct SceneColours
{
	unsigned int background;
	unsigned int circles;
	unsigned int circle_outlines;
};

struct Settings
{
	
	// constructor
	Settings()
	{
		Reset();
	}

	// Reset all of the settings which are stored in the settings struct
	void Reset()
	{
		ResetSceneColours();

		initial_window_width = 1500;
		initial_window_height = 900;

		circle_res = 32;

		simulation_hertz = 120;
	}
	
	// The default scene colours
	void ResetSceneColours()
	{
		scene_colours.background = Palette::JP_DARK_GRAY;
		scene_colours.circles = Palette::JP_RED;
		scene_colours.circle_outlines = Palette::JP_DARK_RED;
	}

	// Scene Colours instance
	SceneColours scene_colours;

	// Window vars
	unsigned int initial_window_width;
	unsigned int initial_window_height;

	// Renderer vars
	int circle_res;

	// Scenes vars
	bool scene_has_changed;
	unsigned int scene_number;

	// Simulation vars
	int simulation_hertz;
};

// This settings struct will be accessable by every file which includes "Settings.hpp".
// It is declared here in "Settings.hpp" and defined in "Settings.cpp"
extern Settings settings;

#endif
