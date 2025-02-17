#ifndef JP_PARTICLES
#define JP_PARTICLES

// C++ libraries
#include <vector>

// glm for vectors and matrices
#include <glm/glm.hpp>

// gives us access to shapes like Line and Circle
#include <shapes.hpp>

class Particle
{
public:
	Circle circle;
	glm::vec2 position;
	glm::vec2 old_position;
	glm::vec2 accelleration;

	Particle(glm::vec2 position, glm::vec2 old_position, glm::vec2 accelleration, glm::vec4 colour, float radius, int layer);
};

class ParticleManager
{
public:
	std::vector<Particle*> particle_stack;

	ParticleManager();
};

#endif
