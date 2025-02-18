#include "glm/detail/type_vec.hpp"
#include <particles.hpp>

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

Spawner::Spawner(std::vector<Particle*>* pointer_to_particle_stack) :
time_at_last_spawn(0.0f),
particles_per_second(3),
max_particle_count(100),
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
	spawner_clock.update();
	
	if (pointer_to_particle_stack->size() < max_particle_count)
	{
		// if its time to make a new particle
		if (time_at_last_spawn + (1.0f / particles_per_second) < spawner_clock.currentTime)
		{
			// Create new particle and push it onto the stack
			Particle* new_particle = new Particle(position, particle_colour, particle_radius);

			pointer_to_particle_stack->push_back(new_particle);

			// add the initial velocity to the particle
			glm::vec2 time_step_vector = glm::vec2(time_step, time_step);
			new_particle->accelerate(initial_velocity * time_step_vector);

			// update time_at_last_spawn
			time_at_last_spawn = spawner_clock.currentTime;
		}
	}
}

ParticleManager::ParticleManager(int simulation_hertz) : spawner(&particle_stack)
{
	// Set up time step & simulation hertz
	this->simulation_hertz = simulation_hertz;
	time_step_in_seconds = 1.0f / (float)simulation_hertz;

	// Set up spawner
	spawner.setParticlesPerSecond(3);
	spawner.setMaxParticleCount(1);
	spawner.setParticleRadius(34.25f);
	spawner.setParticleColour(glm::vec4(1.0f, 0.3f, 0.3f, 1.0f));
	spawner.setParticleInitialVelocity(glm::vec2(-200000.0f, 0.0f));
	spawner.setPosition(glm::vec2(-350.0f, 400.0f));

	// Set up default constraint vars
	constraint_pos = glm::vec2(0.0f, 0.0f);
	constraint_x_scale = 1000;
	constraint_y_scale = 1000;

	// Set up gravity
	gravity = glm::vec2(0.0f, -9806.65f);

	return;
}

void ParticleManager::update()
{
	spawner.update(time_step_in_seconds);
	updateParticles();
	constrainParticles();
	solveCollisions();

	return;
}

void ParticleManager::setConstraint(glm::vec2 pos, float x_scale, float y_scale)
{
	constraint_pos = pos;
	constraint_x_scale = x_scale;
	constraint_y_scale = y_scale;
}

void ParticleManager::updateParticles()
{
	// loop over every particle and update their positions
	for (int i = 0; i < (int)particle_stack.size(); i++)
	{
		particle_stack[i]->accelerate(gravity);
		particle_stack[i]->update(time_step_in_seconds);
	}
}

void ParticleManager::constrainParticles()
{
	// loop over every particle
	for (int i = 0; i < (int)particle_stack.size(); i++)
	{
		float top_wall = constraint_pos.y + (constraint_y_scale / 2);
		float bottom_wall = constraint_pos.y - (constraint_y_scale / 2);
		float left_wall = constraint_pos.x - (constraint_x_scale / 2);
		float right_wall = constraint_pos.x + (constraint_x_scale / 2);

		Particle* particle = particle_stack[i];

		if (particle->position.y + particle->radius > top_wall)
		{
			// get distance between new_pos.y and old_pos.y
			float velocity = particle->position.y - particle->old_position.y;

			// flip edge of circle around the bottom wall
			float edge_of_particle = particle->position.y + particle->radius;
			particle->position -= glm::vec2(0.0f, 2 * (edge_of_particle - top_wall));

			// flip set old_position to be position + velocity (switches the Y velocity around)
			particle->old_position = glm::vec2(particle->old_position.x, particle->position.y + velocity);
		}

		if (particle->position.y - particle->radius < bottom_wall)
		{
			// get distance between new_pos.y and old_pos.y
			float velocity = particle->position.y - particle->old_position.y;

			// flip edge of circle around the bottom wall
			float edge_of_particle = particle->position.y - particle->radius;
			particle->position -= glm::vec2(0.0f, 2 * (edge_of_particle - bottom_wall));

			// flip set old_position to be position + velocity (switches the y velocity around)
			particle->old_position = glm::vec2(particle->old_position.x, particle->position.y + velocity);
		}

		if (particle->position.x - particle->radius < left_wall)
		{
			// get distance between new_pos.y and old_pos.y
			float velocity = particle->position.x - particle->old_position.x;

			// flip edge of circle around the bottom wall
			float edge_of_particle = particle->position.x - particle->radius;
			particle->position -= glm::vec2(2 * (edge_of_particle - left_wall), 0.0f);

			// flip set old_position to be position + velocity (switches the x velocity around)
			particle->old_position = glm::vec2(particle->position.x + velocity, particle->old_position.y);
		}

		if (particle->position.x + particle->radius > right_wall)
		{
			// get distance between new_pos.y and old_pos.y
			float velocity = particle->position.x - particle->old_position.x;

			// flip edge of circle around the bottom wall
			float edge_of_particle = particle->position.x + particle->radius;
			particle->position -= glm::vec2(2 * (edge_of_particle - right_wall), 0.0f);

			// flip set old_position to be position + velocity (switches the Y velocity around)
			particle->old_position = glm::vec2(particle->position.x + velocity, particle->old_position.y);
		}



		// update the position of the circle which reprisents our particle
		particle->circle.position = particle->position;
	}
}

void ParticleManager::solveCollisions()
{
	int particle_count = (int)particle_stack.size();
	for (int n = 0; n < particle_count; n++)
	{
		Particle* particle_one = particle_stack[n];
		for (int i = n+1; i < particle_count; i++)
		{
			Particle* particle_two = particle_stack[i];
			glm::vec2 collision_axis = particle_one->position - particle_two->position;
			float distance = length(collision_axis);

			if (distance < particle_one->radius + particle_two->radius)
			{
				glm::vec2 n = normalize(collision_axis);
				float delta = particle_one->radius + particle_two->radius - distance;
				particle_one->position += n * delta / 2.0f;
				particle_two->position -= n * delta / 2.0f;
			}
		}
	}
}
