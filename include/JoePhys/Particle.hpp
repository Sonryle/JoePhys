#ifndef JP_PARTICLE
#define JP_PARTICLE

typedef float real;

#include "Vec2.hpp"

// Stores a position, velocity, radius and mass
struct Particle
{
	// constructors
	Particle() : pos(0), vel(0), elasticity(0.5f), radius(100), mass(1.0f), is_static(0) {}
	Particle(vec2 pos, vec2 vel, real elasticity, real radius, real mass, bool is_static) : pos(pos), vel(vel), elasticity(elasticity), radius(radius), mass(mass), is_static(is_static) {}

	// functions
	void Accelerate(vec2 force);
	void Accelerate(int runge_kutta_sample, vec2 force);
	vec2 GetAcceleration();
	vec2 GetSpringAcceleration(int runge_kutta_sample);
	void ResetAcceleration();
	void ApplyDrag(real dampening);
	void UpdatePosition(real dt);
	void ResolveCollision(Particle* p);

	
	// variables
	bool is_static;
	vec2 pos;	// in meters
	vec2 vel;	// in meters/second
	real elasticity;// 1 == perfectly elastic | 0 == no elasticity
	real radius;	// in meters
	real mass;	// in grams

private:
	vec2 acc;
	// spring acceleration for runge-kutta samples k1 & k2
	vec2 spring_acc[2];
};

#endif
