#include "JoePhys/Vec2.hpp"
#include <JoePhys/Spring.hpp>

void Spring::Update(double dt)
{
	// for runge kutta sample k1
	// -------------------------

	vec2 diff = particleA->pos - particleB->pos;
	vec2 force = normalize(diff) * (resting_length - length(diff));
	force *= stiffness;

	// Accelerate particle's spring acceleration for runge kutta sample k1
	particleA->spring_acc[0] += force / particleA->mass;
	particleB->spring_acc[0] += -force / particleB->mass;

	// for runge kutta sample k2
	// -------------------------

	diff = (particleA->pos + particleA->vel * dt) - (particleB->pos + particleB->vel * dt);
	force = normalize(diff) * (resting_length - length(diff));
	force *= stiffness;

	// Accelerate particle's spring acceleration for runge kutta sample k2
	particleA->spring_acc[1] += force / particleA->mass;
	particleB->spring_acc[1] += -force / particleB->mass;
}
