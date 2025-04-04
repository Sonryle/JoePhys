#include "JoePhys/Particle.hpp"

void Particle::Accelerate(vec2 force)
{
	if (!is_static)
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
		if (length(vel_in_meters_per_sec) < 0.0001f)
			return;
		if (is_static)
			return;

		real vel_magnitude = length(vel_in_meters_per_sec);
		real drag_magnitude = vel_magnitude * vel_magnitude * dampening;
		vec2 drag_force = normalize(vel_in_meters_per_sec) * -drag_magnitude;

		Accelerate(drag_force);
}

void Particle::UpdatePosition(real dt)
{
	if (is_static)
		return;

	// Update particle position using Runge-Kutta fourth order
	// -------------------------------------------------------
	
	// We do not need to implement runge-kutta on the velocity,
	// because the acceleration remains the same over the time step
	vel_in_meters_per_sec += GetAcceleration() * dt;

	// Returns the derivative of the position (aka velocity) at a
	// certain time, traveling at a certain velocity
	auto f_x = [this] (vec2 slope, real time)
	{
		return slope + (time * GetAcceleration());
	};

	// slope of position (aka velocity) at four different time
	// intervals and velocities
	vec2 x1 = f_x(vel_in_meters_per_sec, 0) * dt;
	vec2 x2 = f_x(vel_in_meters_per_sec + x1/2, dt/2) * dt;
	vec2 x3 = f_x(vel_in_meters_per_sec + x2/2, dt/2) * dt;
	vec2 x4 = f_x(vel_in_meters_per_sec + x3, dt) * dt;

	// Weighted average of four slope gives us a good aproximation
	// of the optimal velocity to move our particle by
	vec2 delta_x = (x1 + 2*x2 + 2*x3 + x4) / 6;

	pos_in_meters += delta_x;

	ResetAcceleration();
}

// Solves collision between itself and another particle
void Particle::ResolveCollision(Particle* other_particle)
{
	Particle* p1 = this;
	Particle* p2 = other_particle;

	// Move particles away from eachother along their impact axis
	// ----------------------------------------------------------
	
	vec2 impact_axis = normalize(p1->pos_in_meters - p2->pos_in_meters);
	real dist_between_particles = length(p1->pos_in_meters - p2->pos_in_meters);
	real overlap = (p1->radius_in_meters + p2->radius_in_meters) - dist_between_particles;

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
	// -----------------------

	real CoR = (p1->elasticity + p2->elasticity) * 0.5f;	// Coefficient of Restitution
	vec2 p1_v = p1->vel_in_meters_per_sec;
	vec2 p2_v = p2->vel_in_meters_per_sec;
	vec2 p1_p = p1->pos_in_meters;
	vec2 p2_p = p2->pos_in_meters;
	real p1_m = p1->mass_in_grams;
	real p2_m = p2->mass_in_grams;
	
	// if particle one is NOT static and particle two IS static
	if (!p1->is_static && p2->is_static)
	{
		// solve for particle one
		real m_ratio = 1+CoR;
		vec2 v_diff = p1_v - p2_v;
		vec2 p_diff = p1_p - p2_p;
		vec2 proj = project(v_diff, p_diff);
		p1->vel_in_meters_per_sec -= (proj * m_ratio);
	}
	// if particle one IS static and particle two is NOT static
	if (p1->is_static && !p2->is_static)
	{
		// solve for particle two
		real m_ratio = 1+CoR;
		vec2 v_diff = p2_v - p1_v;
		vec2 p_diff = p2_p - p1_p;
		vec2 proj = project(v_diff, p_diff);
		p2->vel_in_meters_per_sec -= (proj * m_ratio);
	}
	// if neither particles are static
	if (!p1->is_static && !p2->is_static)
	{
		// solve for particle one
		real m_ratio = ((1+CoR)*p2_m) / (p1_m + p2_m);
		vec2 v_diff = p1_v - p2_v;
		vec2 p_diff = p1_p - p2_p;
		vec2 proj = project(v_diff, p_diff);
		p1->vel_in_meters_per_sec -= (proj * m_ratio);
		
		// solve for particle two
		m_ratio = ((1+CoR)*p1_m) / (p1_m + p2_m);
		v_diff = p2_v - p1_v;
		p_diff = p2_p - p1_p;
		proj = project(v_diff, p_diff);
		p2->vel_in_meters_per_sec -= (proj * m_ratio);
	}
}
