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
	initial_window_width = 1500;
	initial_window_height = 900;

	circle_res = 7;

	frame_limit = 250;

	simulation_hertz = 250;
	sub_steps = 4;
}

