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

	frame_limit = 120;

	enable_gravity = 1;
	enable_drag = 1;
	enable_springs = 1;
	enable_particle_movement = 1;
	enable_collision = 1;
	time_divisor = 1.0f;
	simulation_hertz = 120;
	sub_steps = 4;
}

