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
	UpdateParticlePositions();
	TemporaryConstrainToBox();
}

void World::UpdateParticlePositions()
{
	// TODO: Implement the runge-kutta method
	
	// TEMPORARY::EULER'S INTEGRATION TO MOVE PARTICLES
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

void World::TemporaryConstrainToBox()
{
	// loop over every particle and constrain them
	for (int obj = 0; obj < PhysObjects.size(); obj++)
	{
		PhysObj* current_obj = PhysObjects[obj];
		for (int part = 0; part < current_obj->particles.size(); part++)
		{
			Particle* current_part = current_obj->particles[part];

			// check for collisions with walls of box
			const int box = 250;
			vec2 pos = current_part->position;
			vec2 vel = current_part->velocity;
			
			if (pos.x > box)
			{
				current_part->velocity = vec2(-vel.x, vel.y);
				current_part->position = vec2(box, pos.y);
			}
			if (pos.y > box)
			{
				current_part->velocity = vec2(vel.x, -vel.y);
				current_part->position = vec2(pos.x, box);
			}
			if (pos.x < -box)
			{
				current_part->velocity = vec2(-vel.x, vel.y);
				current_part->position = vec2(-box, pos.y);
			}
			if (pos.y < -box)
			{
				current_part->velocity = vec2(vel.x, -vel.y);
				current_part->position = vec2(pos.x, -box);
			}
		}
	}
}
