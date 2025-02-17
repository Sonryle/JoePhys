#include "glm/detail/type_vec.hpp"
#include <ctime>
#include <particles.hpp>
#include <wayland-util.h>

Particle::Particle(glm::vec2 position, glm::vec4 colour, float radius) :
position(position),
old_position(position),
radius(radius)
{
	circle.colour = colour;
	circle.radius = radius;
	circle.layer = 1;
	return;
}

void Particle::update(double time_step)
{
	const glm::vec2 velocity = position - old_position;

	// update old position
	old_position = position;

	// perform verlet integration
	position = position + velocity + acceleration * glm::vec2(time_step * time_step, time_step * time_step);

	// reset acceleration
	acceleration = glm::vec2(0.0f, 0.0f);
	
	// update the position of the circle which reprisents our particle
	circle.position = position;

	return ;
}

void Particle::accelerate(glm::vec2 force)
{
	acceleration += force;

	return;
}

ParticleManager::ParticleManager(int simulation_hertz)
{
	// Set up time step & simulation hertz
	this->simulation_hertz = simulation_hertz;
	time_step_in_seconds = 1.0f / (float)simulation_hertz;

	// Set up gravity
	gravity = glm::vec2(0.0f, -800.0f);

	// TEMPORARY CREATE A PARTICLE
	float radius = 50;
	glm::vec2 pos = glm::vec2(0.0f, 0.0f);
	glm::vec4 colour = glm::vec4(0.3f, 1.0f, 0.3f, 1.0f);
	Particle* new_particle = new Particle(pos, colour, radius);

	particle_stack.push_back(new_particle);

	return;
}

void ParticleManager::update()
{
	// loop over every particle and update their positions
	for (int i = 0; i < (int)particle_stack.size(); i++)
	{
		particle_stack[i]->accelerate(gravity);
		particle_stack[i]->update(time_step_in_seconds);
	}

	return;
}
