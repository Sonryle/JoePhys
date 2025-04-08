#include <JoePhys/Spring.hpp>

void Spring::Update()
{
	if (is_broken)
		return;

	vec2 dp = particleA->pos_in_meters - particleB->pos_in_meters;
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

	particleA->Accelerate(force);
	particleB->Accelerate(-force);
}
