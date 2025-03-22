#include <JoePhys/Spring.hpp>

void Spring::Update()
{
	vec2 dp = particleA->pos_in_meters - particleB->pos_in_meters;
	real dist = length(dp);
	real diff = resting_length - dist;
	vec2 dn = normalize(dp);

	real k = stiffness;
	vec2 x = dn * diff;
	vec2 force = k * x;

	particleA->Accelerate(force);
	particleB->Accelerate(-force);
}
