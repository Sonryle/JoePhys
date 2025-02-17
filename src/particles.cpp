#include "glm/detail/type_vec.hpp"
#include <particles.hpp>

Particle::Particle(glm::vec2 position, glm::vec2 old_position, glm::vec2 accelleration, glm::vec4 colour, float radius, int layer) :
position(position),
old_position(old_position),
accelleration(accelleration),
radius(radius)
{
	circle.position = position;
	circle.colour = colour;
	circle.radius = radius;
	circle.layer = layer;
	return;
}

void Particle::update(double time_step)
{
	position = glm::vec2(position.x, position.y - 0.1);
	
	// update the position of the circle which reprisents our particle
	circle.position = position;

	return ;
}

ParticleManager::ParticleManager(int simulation_hertz)
{
	// Set up time step & simulation hertz
	this->simulation_hertz = simulation_hertz;
	time_step_in_seconds = 1.0f / (float)simulation_hertz;

	glm::vec2 position = glm::vec2(-100.0f, 0.0f);
	glm::vec2 old_position = glm::vec2(0.0f, 0.0f);
	glm::vec2 accelleration = glm::vec2(0.0f, 0.0f);
	glm::vec4 colour = glm::vec4(0.3f, 0.3f, 1.0f, 1.0f);
	float radius = 50.0f;
	int layer = 1;

	Particle* new_particle = new Particle(position, old_position, accelleration, colour, radius, layer);

	particle_stack.push_back(new_particle);


	position = glm::vec2(100.0f, 0.0f);
	old_position = glm::vec2(0.0f, 0.0f);
	accelleration = glm::vec2(0.0f, 0.0f);
	colour = glm::vec4(0.3f, 1.0f, 0.3f, 1.0f);
	radius = 50.0f;
	layer = 1;

	Particle* new_particle_two = new Particle(position, old_position, accelleration, colour, radius, layer);

	particle_stack.push_back(new_particle_two);

	return;
}

void ParticleManager::update()
{
	// loop over every particle and update their positions
	for (int i = 0; i < (int)particle_stack.size(); i++)
	{
		particle_stack[i]->update(time_step_in_seconds);
	}

	return;
}
