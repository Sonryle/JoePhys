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
	is_fullscreen = 0;
	min_particle_size = 0.1f;
	max_particle_size = 5.0f;

	gravity.Set(0.0f, 0.0f);
	enable_gravity = 1;
	enable_drag = 1;
	enable_springs = 1;
	enable_particle_movement = 1;
	enable_collision = 1;
	repulsion_tool_strength = 10;
	repulsion_tool_radius = 10;
	attraction_tool_strength = 10;
	attraction_tool_radius = 10;
	time_divisor = 1.0f;
	simulation_hertz = 120;
	sub_steps = 8;
}

