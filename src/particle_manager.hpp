#ifndef JP_PARTICLE_MANAGER
#define JP_PARTICLE_MANAGER

// C++ libraries
#include <vector>

// glm for vectors and matrices
#include <glm/glm.hpp>

// include JoePhys particle & particle spawner headers
#include <particle.hpp>
#include <particle_spawner.hpp>

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
