#ifndef JP_SPRING
#define JP_SPRING

#include "Particle.hpp"

struct Spring
{
	Spring(Particle* pA, Particle* pB, real length) : particleA(pA), particleB(pB), length(length) {}
	~Spring() {}
	
	real length;
	Particle* particleB;
	Particle* particleA;
};

#endif
