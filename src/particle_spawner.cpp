#include "glm/detail/type_vec.hpp"
#include <particle_spawner.hpp>

Spawner::Spawner(std::vector<Particle*>* pointer_to_particle_stack) :
time_since_last_spawn(0.0f),
particles_per_second(3),
max_particle_count(100),
particle_elasticity(0.5),
particle_radius(50.0f)
{
	position = glm::vec2(0.0f, 0.0f);
	initial_velocity = glm::vec2(0.0f, 0.0f);
	particle_colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->pointer_to_particle_stack = pointer_to_particle_stack;

	return;
}

void Spawner::setParticlesPerSecond(float particles_per_second)
{
	this->particles_per_second = particles_per_second;
}

void Spawner::setMaxParticleCount(int max_particle_count)
{
	this->max_particle_count = max_particle_count;
}

void Spawner::setParticleElasticity(float elasticity)
{
	this->particle_elasticity = elasticity;
}

void Spawner::setParticleRadius(float radius)
{
	this->particle_radius = radius;
}

void Spawner::setParticleColour(glm::vec4 colour)
{
	this->particle_colour = colour;
}

void Spawner::setParticleInitialVelocity(glm::vec2 velocity)
{
	initial_velocity = velocity;
}

void Spawner::setPosition(glm::vec2 position)
{
	this->position = position;
}

void Spawner::update(double time_step)
{
	time_since_last_spawn += time_step;
	
	if (pointer_to_particle_stack->size() < max_particle_count)
	{
		// if its time to make a new particle
		if (time_since_last_spawn >= 1 / particles_per_second)
		{
			// Create new particle and push it onto the stack
			
			// TEMPORARY CODE TO MAKE THE FIRST PARTICLE RED AND THE SECOND ONE BLUE
			glm::vec4 temp_col = particle_colour;
			if ((int)pointer_to_particle_stack->size() == 0)
				temp_col = glm::vec4(0.7f, 0.5f, 0.5f, 1.0f);
			if ((int)pointer_to_particle_stack->size() == 1)
				temp_col = glm::vec4(0.5f, 0.5f, 0.7f, 1.0f);

			Particle* new_particle = new Particle(position, temp_col, particle_elasticity, particle_radius);

			pointer_to_particle_stack->push_back(new_particle);

			// add the initial velocity to the particle
			new_particle->old_position = new_particle->position - (initial_velocity * glm::vec2(time_step));

			// reset elapsed time
			time_since_last_spawn = 0;
		}
	}
}
