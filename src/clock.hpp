#ifndef CLOCK_JP
#define CLOCK_JP

// GLFW for glfwGetTime()
#include <GLFW/glfw3.h>

class Clock
{
public:
	
	double currentTime;
	double deltaTime;

	Clock();
	// ----------------------------------------------------
	void update();
	// ----------------------------------------------------

private:

	double prevTime;
};

#endif
