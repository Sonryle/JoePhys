#ifndef JP_SPRING
#define JP_SPRING

#include "Particle.hpp"

struct Spring
{
	Spring(Particle* pA, Particle* pB, real length, real stiffness, real tearing_length, real crushing_length) : particleA(pA), particleB(pB), resting_length(length), stiffness(stiffness), tearing_length(tearing_length), crushing_length(crushing_length), is_broken(0) {}
	~Spring() {}

	void Update();
	
	real resting_length;
	real stiffness;
	real tearing_length;
	real crushing_length;
	bool is_broken;
	Particle* particleB;
	Particle* particleA;
};

#endif
