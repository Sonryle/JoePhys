#ifndef JP_SETTINGS
#define JP_SETTINGS

struct Settings
{
	unsigned int initial_window_width;
	unsigned int initial_window_height;

	Settings()
	{
		Reset();
	}

	void Reset()
	{
		initial_window_width = 1500;
		initial_window_height = 900;
	}

};

extern Settings settings;

#endif
