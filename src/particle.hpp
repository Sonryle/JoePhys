#ifndef JP_PARTICLES
#define JP_PARTICLES

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
	glm::vec2 acceleration;
	float radius;

	Particle(glm::vec2 position, glm::vec4 colour, float radius);
	// --------------------------------------------------------
	void update(double time_step);
	// --------------------------------------------------------
	void accelerate(glm::vec2 force);
	// --------------------------------------------------------
};

#endif
