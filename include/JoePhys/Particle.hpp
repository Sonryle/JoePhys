#ifndef JP_PARTICLE
#define JP_PARTICLE

typedef float real;

#include "Vec2.hpp"

// Stores a position, velocity, radius and mass
struct Particle
{
	// constructors
	Particle() : position(0), velocity(0), radius(100), mass(1.0f) {}
	Particle(vec2 pos, vec2 vel, real radius, real mass) : position(pos), velocity(vel), radius(radius), mass(mass) {}
	
	// variables
	vec2 position;
	vec2 velocity;
	real radius;
	real mass;
};

#endif
