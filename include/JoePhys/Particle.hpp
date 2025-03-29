#ifndef JP_PARTICLE
#define JP_PARTICLE

typedef float real;

#include "Vec2.hpp"

// Stores a position, velocity, radius and mass
struct Particle
{
	// constructors
	Particle() : pos_in_meters(0), vel_in_meters_per_sec(0), elasticity(0.5f), radius_in_meters(100), mass_in_grams(1.0f), is_static(0) {}
	Particle(vec2 pos, vec2 vel, real elasticity, real radius, real mass, bool is_static) : pos_in_meters(pos), vel_in_meters_per_sec(vel), elasticity(elasticity), radius_in_meters(radius), mass_in_grams(mass), is_static(is_static) {}

	// functions
	void Accelerate(vec2 force);
	vec2 GetAcceleration();
	void ResetAcceleration();
	void ApplyDrag(real dampening);
	void UpdatePosition(real dt);
	void ResolveCollision(Particle* p);

	
	// variables
	bool is_static;
	vec2 pos_in_meters;
	vec2 vel_in_meters_per_sec;
	real elasticity;
	real radius_in_meters;
	real mass_in_grams;

private:
	vec2 acc_in_meters_per_sec;
};

#endif
