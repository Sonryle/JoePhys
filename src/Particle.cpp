#include "JoePhys/Particle.hpp"

void Particle::Accelerate(vec2 force)
{
	acc_in_meters_per_sec += force / mass_in_grams;
}

vec2 Particle::GetAcceleration()
{
	return acc_in_meters_per_sec;
}

void Particle::ResetAcceleration()
{
	acc_in_meters_per_sec.Set(0.0f, 0.0f);
}

void Particle::ApplyDrag(real dampening)
{
		if (vel_in_meters_per_sec.GetLength() < 0.0001f)
			return;
		if (is_static)
			return;

		real vel_magnitude = vel_in_meters_per_sec.GetLength();
		real drag_magnitude = vel_magnitude * vel_magnitude * dampening;
		vec2 drag_force = vel_in_meters_per_sec.GetNormalized() * -drag_magnitude;

		Accelerate(drag_force);
}

void Particle::UpdatePosition(real dt)
{
	// if particle is static, skip it
	if (is_static)
	{
		ResetAcceleration();
		return;
	}

	// since acceleration remains constant over time step, velocity
	// can be updated using euler's integration
	vel_in_meters_per_sec += GetAcceleration() * dt;

	vec2 v = vel_in_meters_per_sec;
	vec2 a = GetAcceleration();

	vec2 k1 = v;
	vec2 k2 = v + (a * dt / 2.0f);
	vec2 k3 = v + (a * dt / 2.0f);
	vec2 k4 = v + (a * dt);

	pos_in_meters += ((k1 + 2.0f*k2 + 2.0f*k3 + k4) / 6.0f) * dt;
	ResetAcceleration();
}

// Solves collision between itself and another particle
void Particle::ResolveCollision(Particle* other_particle)
{
	Particle* p1 = this;
	Particle* p2 = other_particle;
	vec2 impact_axis = normalize(p1->pos_in_meters - p2->pos_in_meters);
	real dist_between_parts = length(p1->pos_in_meters - p2->pos_in_meters);

	// Move particles away from eachother along their impact axis
	// ----------------------------------------------------------
	
	real overlap = (p1->radius_in_meters + p2->radius_in_meters) - dist_between_parts;

	// consider whether each particle is static or dynamic.
	if ( p1->is_static && !p2->is_static)
		p2->pos_in_meters -= impact_axis * overlap;
	if (!p1->is_static &&  p2->is_static)
		p1->pos_in_meters += impact_axis * overlap;
	if (!p1->is_static && !p2->is_static)
	{
		p1->pos_in_meters += impact_axis * overlap * 0.5f;
		p2->pos_in_meters -= impact_axis * overlap * 0.5f;
	}
	

	// Solve for new velocities
	// ------------------------
	
	real coefficient_of_restituation = (p1->elasticity + p2->elasticity) * 0.5f;
	// VCoM = Velocity of Center of Mass
	vec2 VCoM
}
