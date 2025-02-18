#ifndef JP_PARTICLES
#define JP_PARTICLES

// C++ libraries
#include <vector>

// glm for vectors and matrices
#include <glm/glm.hpp>

// gives us access to shapes like Line and Circle
#include <shapes.hpp>

// add the JoePhys clock class
#include <clock.hpp>

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

class Spawner
{
public:

	std::vector<Particle*>* pointer_to_particle_stack;

	Clock spawner_clock;
	double time_at_last_spawn;
	float particles_per_second;
	float particle_radius;
	glm::vec2 position;
	glm::vec4 particle_colour;

	Spawner(std::vector<Particle*>* pointer_to_particle_stack);
	// ---------------------------------------------------
	void setParticlesPerSecond(float particles_per_second);
	// ---------------------------------------------------
	void setParticleRadius(float radius);
	// ---------------------------------------------------
	void setParticleColour(glm::vec4 colour);
	// ---------------------------------------------------
	void setPosition(glm::vec2 position);
	// ---------------------------------------------------
	void update();
	// ---------------------------------------------------
};

class ParticleManager
{
public:

	std::vector<Particle*> particle_stack;
	glm::vec2 gravity;

	Spawner spawner;

	// amount of SIMULATED time that the simulation will move forward by every update
	float time_step_in_seconds;
	// defines how many times per SIMULATED second the simulation updates (must match framerate for real-time physics)
	int simulation_hertz;

	ParticleManager(int simulation_hertz);
	// -----------------------------------------
	void update();
	// -----------------------------------------
};

#endif
