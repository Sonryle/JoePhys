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
	real dist_between_particles = length(p1->pos_in_meters - p2->pos_in_meters);
	real PI = 3.141592653589;

	// Move particles away from eachother along their impact axis
	// ----------------------------------------------------------
	
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
	// ------------------------
	
	real mass_sum = p1->mass_in_grams + p2->mass_in_grams;
	real mass_diff = p1->mass_in_grams - p2->mass_in_grams;
	real impact_angle = angleInRadians(impact_axis);
	
	real p1m = p1->mass_in_grams;
	real p2m = p2->mass_in_grams;
	real p1v = length(p1->vel_in_meters_per_sec);
	real p2v = length(p2->vel_in_meters_per_sec);
	real p1dir = angleInRadians(p1->vel_in_meters_per_sec);
	real p2dir = angleInRadians(p2->vel_in_meters_per_sec);
	
	// Solve for the first particle
	real p1numerator = p1v * cos(p1dir - impact_angle) * mass_diff + 2.0f * p2m * p2v * cos(p2dir - impact_angle);
	real p1denominator = mass_sum;
	real p1x = p1numerator / p1denominator * cos(impact_angle) + p1v * sin(p1dir - impact_angle) * cos(impact_angle + PI * 0.5f);
	real p1y = p1numerator / p1denominator * sin(impact_angle) + p1v * sin(p1dir - impact_angle) * sin(impact_angle + PI * 0.5f);
	if (!p1->is_static)
		p1->vel_in_meters_per_sec.Set(p1x, p1y);

	// Solve for second particle
	real p2numerator = p2v * cos(p2dir - impact_angle) * mass_diff + 2.0f * p1m * p1v * cos(p1dir - impact_angle);
	real p2denominator = mass_sum;
	real p2x = p2numerator / p2denominator * cos(impact_angle) + p2v * sin(p2dir - impact_angle) * cos(impact_angle + PI * 0.5f);
	real p2y = p2numerator / p2denominator * sin(impact_angle) + p2v * sin(p2dir - impact_angle) * sin(impact_angle + PI * 0.5f);
	if (!p2->is_static)
		p2->vel_in_meters_per_sec.Set(p2x, p2y);
}
