#ifndef JP_PHYS_OBJECT
#define JP_PHYS_OBJECT

#include <vector>
#include "Particle.hpp"

// Physics Objects (PhysObj) will store a group of particles and optionally
// store springs and/or rods which will hold the particles together.
struct PhysObj
{
	// constructors & destructor
	PhysObj() {}
	~PhysObj()
	{
		// loop through every particle and delete it
		for (int n = 0; n < particles.size(); n++)
			delete particles[n];
	}

	// variables
	std::vector<Particle*> particles;
};

#endif
