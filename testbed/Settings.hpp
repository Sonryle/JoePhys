#ifndef JP_SETTINGS
#define JP_SETTINGS

typedef float real;

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

	// Simulation vars
	bool enable_gravity;
	bool enable_drag;
	bool enable_springs;
	bool enable_particle_movement;
	bool enable_collision;
	int time_divisor;
	int simulation_hertz;
	int sub_steps;
};

// This settings struct will be accessable by every file which includes "Settings.hpp".
// It is declared here in "Settings.hpp" and defined in "Settings.cpp"
extern Settings settings;

#endif
