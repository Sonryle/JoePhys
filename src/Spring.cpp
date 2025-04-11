#include <JoePhys/Spring.hpp>

void Spring::Update(double dt)
{
	if (is_broken)
		return;

	// for runge kutta sample k1
	// -------------------------

	vec2 dp = particleA->pos - particleB->pos;
	real dist = length(dp);
	real diff = resting_length - dist;
	vec2 dn = normalize(dp);

	if (dist > tearing_length && tearing_length != -1)
		is_broken = 1;
	if (dist < crushing_length && crushing_length != -1)
		is_broken = 1;

	real k = stiffness;
	vec2 x = dn * diff;
	vec2 force = k * x;

	// Accelerate particle's spring acceleration for runge kutta sample k1
	particleA->Accelerate(1, force);
	particleB->Accelerate(1, -force);

	// for runge kutta sample k2
	// -------------------------

	dp = (particleA->pos + particleA->vel * dt) - (particleB->pos + particleB->vel * dt);
	dist = length(dp);
	diff = resting_length - dist;
	dn = normalize(dp);

	if (dist > tearing_length && tearing_length != -1)
		is_broken = 1;
	if (dist < crushing_length && crushing_length != -1)
		is_broken = 1;

	k = stiffness;
	x = dn * diff;
	force = k * x;

	// Accelerate particle's spring acceleration for runge kutta sample k2
	particleA->Accelerate(2, force);
	particleB->Accelerate(2, -force);
}
