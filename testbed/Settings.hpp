#ifndef JP_SETTINGS
#define JP_SETTINGS

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
	int simulation_hertz;
	int sub_steps;
};

// This settings struct will be accessable by every file which includes "Settings.hpp".
// It is declared here in "Settings.hpp" and defined in "Settings.cpp"
extern Settings settings;

#endif
