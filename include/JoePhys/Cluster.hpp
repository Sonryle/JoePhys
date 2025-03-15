#ifndef CLUSTER
#define CLUSTER

#include <vector>
#include "Particle.hpp"

struct Cluster
{
	Cluster() {}
	~Cluster()
	{
		// Delete all particles in cluster
		for (Particle* n : particles)
			delete n;
	}

	std::vector<Particle*> particles;
};

#endif
