#include <clock.hpp>

Clock::Clock() : currentTime(0.0f), deltaTime(0.0f), prevTime(0.0f)
{
	return;
}

void Clock::update()
{
	prevTime = currentTime;
	currentTime = glfwGetTime();
	deltaTime = currentTime - prevTime;
	
	return;
}
