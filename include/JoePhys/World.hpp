#ifndef JP_WORLD
#define JP_WORLD

typedef float real;

#include <vector>
#include <JoePhys/Cluster.hpp>

// enum containing options for the "Step()" function
enum world_options
{
	NO_GRAVITY		= 0b00001,
	NO_DRAG			= 0b00010,
	NO_PARTICLE_MOVEMENT	= 0b00100,
	NO_PARTICLE_COLLISIONS 	= 0b01000,
	NO_SPRING_FORCES	= 0b10000
};

// World class will store a collection of Physics Objects (PhysObj classes).
// It will have a "step" function which will update the position of all particles
// in the PhysObjects and constrain them to the springs and/or rods holding them
// together.
struct World
{
	// constructors & destuctor
	World() {}
	~World();

	// functions
	void Create(int simulation_hertz, int sub_steps, vec2 gravity);
	void Step(int flags);

	// variables
	std::vector<Cluster*> clusters;
	int simulation_hertz;
	int sub_steps;
private:

	// functions
	void ApplyGravityToParticles();
	void ApplyDragToParticles();
	void UpdateParticlePositions(real dt);
	void ResolveAllCollisions();
	void UpdateSprings(real dt);

	// variables
	vec2 gravity;
};

#endif
