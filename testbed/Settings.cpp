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

	frame_limit = 120;

	simulation_hertz = 120;
}

void Settings::ResetSceneColours()
{
	scene_colours.background = Palette::JP_DARK_GRAY;
	scene_colours.circles = Palette::JP_RED;
	scene_colours.circle_outlines = Palette::JP_DARK_RED;
}
