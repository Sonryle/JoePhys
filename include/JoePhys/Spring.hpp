#ifndef JP_SPRING
#define JP_SPRING

#include "Particle.hpp"

struct Spring
{
	Spring(Particle* pA, Particle* pB, real length) : particleA(pA), particleB(pB), resting_length(length) {}
	~Spring() {}
	
	real resting_length;
	Particle* particleB;
	Particle* particleA;
};

#endif
