#include <JoePhys/Spring.hpp>

void Spring::Update(double dt)
{
	// for runge kutta sample k1
	// -------------------------

	vec2 dp = particleA->pos - particleB->pos;
	real dist = length(dp);
	real diff = resting_length - dist;
	vec2 dn = normalize(dp);

	real k = stiffness;
	vec2 x = dn * diff;
	vec2 force = k * x;

	// Accelerate particle's spring acceleration for runge kutta sample k1
	particleA->spring_acc[0] += force / particleA->mass;
	particleB->spring_acc[0] += -force / particleB->mass;

	// for runge kutta sample k2
	// -------------------------

	dp = (particleA->pos + particleA->vel * dt) - (particleB->pos + particleB->vel * dt);
	dist = length(dp);
	diff = resting_length - dist;
	dn = normalize(dp);

	k = stiffness;
	x = dn * diff;
	force = k * x;

	// Accelerate particle's spring acceleration for runge kutta sample k2
	particleA->spring_acc[1] += force / particleA->mass;
	particleB->spring_acc[1] += -force / particleB->mass;
}
