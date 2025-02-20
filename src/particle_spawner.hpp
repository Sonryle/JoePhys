#ifndef JP_PARTICLE_SPAWNER
#define JP_PARTICLE_SPAWNER

// C++ libraries
#include <vector>

// include JoePhys particle class header
#include <particle.hpp>

// glm for vectors and matrices
#include <glm/glm.hpp>

class Spawner
{
public:

	std::vector<Particle*>* pointer_to_particle_stack;

	double time_since_last_spawn;
	float particles_per_second;
	int max_particle_count;
	float particle_elasticity;
	float particle_radius;
	glm::vec2 position;
	glm::vec2 initial_velocity;
	glm::vec4 particle_colour;

	Spawner(std::vector<Particle*>* pointer_to_particle_stack);
	// ---------------------------------------------------
	void setParticlesPerSecond(float particles_per_second);
	// ---------------------------------------------------
	void setMaxParticleCount(int max_particle_count);
	// ---------------------------------------------------
	void setParticleElasticity(float elasticity);
	// ---------------------------------------------------
	void setParticleRadius(float radius);
	// ---------------------------------------------------
	void setParticleColour(glm::vec4 colour);
	// ---------------------------------------------------
	void setParticleInitialVelocity(glm::vec2 velocity);
	// ---------------------------------------------------
	void setPosition(glm::vec2 position);
	// ---------------------------------------------------
	void update(double time_step);
	// ---------------------------------------------------
};

#endif
