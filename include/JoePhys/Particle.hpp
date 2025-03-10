#ifndef JP_PARTICLE
#define JP_PARTICLE

typedef float real;

#include "Vec2.hpp"

// Stores a position, velocity, radius and mass
struct Particle
{
	// constructors
	Particle() : position(0), velocity(0), elasticity(0.5f), radius(100), mass(1.0f) {}
	Particle(vec2 pos, vec2 vel, real elasticity, real radius, real mass) : position(pos), velocity(vel), elasticity(elasticity), radius(radius), mass(mass) {}
	
	// variables
	vec2 position;
	vec2 velocity;
	vec2 acceleration;
	real elasticity;
	real radius;
	real mass;
};

#endif
