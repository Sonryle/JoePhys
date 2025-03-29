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

		real vel_magnitude = vel_in_meters_per_sec.GetLength();
		real drag_magnitude = vel_magnitude * vel_magnitude * dampening;
		vec2 drag_force = vel_in_meters_per_sec.GetNormalized() * -drag_magnitude;

		Accelerate(drag_force);
}

void Particle::UpdatePosition(real dt)
{
	// if particle is static, skip it
	if (is_static == 1)
		return;

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

// Solves collision between two particles, pA & pB
void Particle::ResolveCollision(Particle* p)
{
	// Credit for Particle Collision Resolution goes to "The Coding Train"
	// https://www.youtube.com/watch?v=dJNFPv9Mj-Y&t=1421s
	
	// If dist between particles is less than their radii then they have collided
	real dist = length(pos_in_meters - p->pos_in_meters);
	if (dist < radius_in_meters + p->radius_in_meters)
	{
		// difference in position between particles
		vec2 pos_diff = p->pos_in_meters - pos_in_meters;

		// Move particles away from eachother along their impact axis
		// ----------------------------------------------------------

		vec2 dir = pos_diff.GetNormalized();
		real overlap = dist - (radius_in_meters + p->radius_in_meters);
		dir *= overlap * 0.5f;
		// if either particles are static, move opposite particle accordingly
		if (!is_static && !p->is_static)
		{
			pos_in_meters += dir;
			p->pos_in_meters -= dir;
		}
		else if (is_static && !p->is_static)
			p->pos_in_meters -= dir * 2.0f;
		else if (!is_static && p->is_static)
			pos_in_meters += dir * 2.0f;

		// Solve for new velocities
		// ------------------------

		real pAMass = mass_in_grams;
		real pBMass = p->mass_in_grams;

		// change mass to be insanely high if object is static
		// so that most of the energy is kept by the object
		// colliding with it (i know its not the best solution)
		
		/* if (is_static) */
		/* 	pAMass = 100000000000000.0f; */
		/* if (p->is_static) */
		/* 	pBMass = 100000000000000.0f; */

		// Particle A

		real mass_sum;
		vec2 vel_diff;
		real denominator;
		real numerator;
		vec2 delta_vel;

		mass_sum = pAMass + pBMass;
		vel_diff = p->vel_in_meters_per_sec - vel_in_meters_per_sec;
		denominator = mass_sum * dist * dist;
		numerator = 2 * pBMass * dot(vel_diff, pos_diff);
		delta_vel = pos_diff * (numerator / denominator);
		if (!is_static)
			vel_in_meters_per_sec += delta_vel * elasticity;

		// Particle B
		vel_diff *= -1;
		pos_diff *= -1;
		numerator = 2 * pAMass * dot(vel_diff, pos_diff);
		delta_vel = pos_diff * (numerator/ denominator);
		if (!p->is_static)
			p->vel_in_meters_per_sec+= delta_vel * p->elasticity;

	}
}
