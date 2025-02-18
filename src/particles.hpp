#ifndef JP_PARTICLES
#define JP_PARTICLES

// C++ libraries
#include <vector>
#include <iostream>

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
	int max_particle_count;
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

class ParticleManager
{
public:

	std::vector<Particle*> particle_stack;

	ParticleManager(int simulation_hertz);
	// -----------------------------------------
	void update();
	// -----------------------------------------
	void setConstraint(glm::vec2 pos, float x_scale, float y_scale);
	// -----------------------------------------
	
private:

	Spawner spawner;
	glm::vec2 gravity;
	glm::vec2 constraint_pos;
	float constraint_x_scale;
	float constraint_y_scale;

	// amount of SIMULATED time that the simulation will move forward by every update
	float time_step_in_seconds;
	// defines how many times per SIMULATED second the simulation updates (must match framerate for real-time physics)
	int simulation_hertz;

	void updateParticles();
	// -----------------------------------------
	void constrainParticles();
	// -----------------------------------------
	void solveCollisions();
	// -----------------------------------------
};

#endif
