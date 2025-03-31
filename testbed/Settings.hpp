#ifndef JP_SETTINGS
#define JP_SETTINGS

typedef float real;

#include "JoePhys/Vec2.hpp"

struct Settings
{
	
	// constructor
	Settings();

	// functions
	void Reset();

	// VARIABLES
	// ---------

	// Window vars
	unsigned int initial_window_width;
	unsigned int initial_window_height;

	// Renderer vars
	int circle_res;

	// Main Application Vars
	int frame_limit;
	bool is_fullscreen;
	real min_particle_size;
	real max_particle_size;

	// Simulation vars
	vec2 gravity;
	bool enable_gravity;
	bool enable_drag;
	bool enable_springs;
	bool enable_particle_movement;
	bool enable_collision;
	real repulsion_tool_strength;
	real attraction_tool_strength;
	int time_divisor;
	int simulation_hertz;
	int sub_steps;
};

// This settings struct will be accessable by every file which includes "Settings.hpp".
// It is declared here in "Settings.hpp" and defined in "Settings.cpp"
extern Settings settings;

#endif
