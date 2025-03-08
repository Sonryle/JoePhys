#ifndef JP_WORLD
#define JP_WORLD

typedef float real;

#include <vector>
#include <JoePhys/PhysObj.hpp>

// World class will store a collection of Physics Objects (PhysObj classes).
// It will have a "step" function which will update the position of all particles
// in the PhysObjects and constrain them to the springs and/or rods holding them
// together.
struct World
{
	// constructors & destuctor
	World(int simulation_hertz, vec2 gravity);
	~World();

	// functions
	void Step();

	// variables
	std::vector<PhysObj*> PhysObjects;
private:

	// functions
	void UpdateParticlePositions();

	// variables
	vec2 gravity;
	int simulation_hertz;
};

#endif
