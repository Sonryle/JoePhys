#include <particle.hpp>

Particle::Particle(glm::vec2 position, glm::vec4 colour, float elasticity, float radius) :
position(position),
old_position(position),
elasticity(elasticity),
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
	const glm::vec2 time_step_vec = glm::vec2(time_step);

	// update old position
	old_position = position;

	// perform verlet integration
	position = position + velocity + acceleration * (time_step_vec * time_step_vec);

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
