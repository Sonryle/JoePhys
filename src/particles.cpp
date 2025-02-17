#include "glm/detail/type_vec.hpp"
#include <particles.hpp>

Particle::Particle(glm::vec2 position, glm::vec2 old_position, glm::vec2 accelleration, glm::vec4 colour, float radius, int layer) :
position(position),
old_position(old_position),
accelleration(accelleration)
{
	circle.colour = colour;
	circle.radius = radius;
	circle.layer = layer;
	return;
}

ParticleManager::ParticleManager()
{
	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	glm::vec2 old_position = glm::vec2(0.0f, 0.0f);
	glm::vec2 accelleration = glm::vec2(0.0f, 0.0f);
	glm::vec4 colour = glm::vec4(0.3f, 0.3f, 1.0f, 1.0f);
	float radius = 50.0f;
	int layer = 1;
	Particle* new_particle = new Particle(position, old_position, accelleration, colour, radius, layer);
	particle_stack.push_back(new_particle);
}
