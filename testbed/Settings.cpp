// Define 'settings' in a source file so that it can be accessed by
// any other file which includes "Settings.hpp" (since 'settings' is declared
// as an extern Settings in "Settings.hpp", but not initialised. It is
// initialised here in "Settings.cpp")
#include "Settings.hpp"

Settings settings;


Settings::Settings()
{
	Reset();
}

void Settings::Reset()
{
	ResetSceneColours();

	initial_window_width = 1500;
	initial_window_height = 900;

	circle_res = 7;

	frame_limit = 250;

	simulation_hertz = 250;
	sub_steps = 8;
}

void Settings::ResetSceneColours()
{
	scene_colours.background = Palette::JP_DARK_GRAY;
	scene_colours.spring = Palette::JP_GREEN;
	scene_colours.particle = Palette::JP_RED;
	scene_colours.particle_outline = Palette::JP_DARK_RED;
	scene_colours.static_particle = Palette::JP_BLUE;
	scene_colours.static_particle_outline = Palette::JP_DARK_BLUE;
}
