#include <cstdio>	// for "stderr" file path constant

#include "JoePhys/World.hpp"

World::World(int simulation_hertz, vec2 gravity)
{
	this->simulation_hertz = simulation_hertz;
	this->gravity = gravity;
}

World::~World()
{
	// loop through every Physics Object and delete it
	for (int n = 0; n < PhysObjects.size(); n++)
		delete PhysObjects[n];
}

void World::Step()
{
	// Loop over every particle and update their position using the
	// runge-kutta method
	
	// TODO: Implement the runge-kutta method
	
	// TEMPORARY:: MOVE PARTICLES DOWN BY 1 EVERY STEP
	for (int obj = 0; obj < PhysObjects.size(); obj++)
	{
		PhysObj* current_obj = PhysObjects[obj];
		for (int part = 0; part < current_obj->particles.size(); part++)
		{
			Particle* current_part = current_obj->particles[part];
			current_part->position += current_part->velocity;
		}
	}
}
