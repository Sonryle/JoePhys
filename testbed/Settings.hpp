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
	unsigned int spring;
	unsigned int particle;
	unsigned int particle_outline;
	unsigned int static_particle;
	unsigned int static_particle_outline;
};

struct Settings
{
	
	// constructor
	Settings();

	// functions
	void Reset();
	void ResetSceneColours();

	// VARIABLES
	// ---------

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

	// Main Application Vars
	int frame_limit;

	// Simulation vars
	int simulation_hertz;
	int sub_steps;
};

// This settings struct will be accessable by every file which includes "Settings.hpp".
// It is declared here in "Settings.hpp" and defined in "Settings.cpp"
extern Settings settings;

#endif
