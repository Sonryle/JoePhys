#ifndef JP_SPRING
#define JP_SPRING

#include "Particle.hpp"

struct Spring
{
	Spring(Particle* pA, Particle* pB, real length, real stiffness) : particleA(pA), particleB(pB), resting_length(length), stiffness(stiffness) {}
	~Spring() {}
	
	real resting_length;
	real stiffness;
	Particle* particleB;
	Particle* particleA;
};

#endif
