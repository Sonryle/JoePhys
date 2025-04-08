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
		if (is_static || length(vel_in_meters_per_sec) < 0.0001f)
			return;

		real vel_magnitude = length(vel_in_meters_per_sec);
		real drag_magnitude = vel_magnitude * vel_magnitude * dampening;
		vec2 drag_force = normalize(vel_in_meters_per_sec) * -drag_magnitude;

		Accelerate(drag_force);
}

vec2 ComputeSpringForce(vec2 pos, vec2 vel)
{
	vec2 anchor_position(0.0f, 0.0f);
	real spring_constant = 1;
	real damping_coefficient = 0.5f;

	vec2 spring_anchor = anchor_position; // fixed point
	real k = spring_constant;             // stiffness
	real d = damping_coefficient;         // damping

	vec2 displacement = pos - spring_anchor;
	vec2 spring_force = -k * displacement;
	vec2 damping_force = -d * vel;

	return spring_force + damping_force;
}

void Particle::UpdatePosition(real dt)
{
	if (is_static)
		return;

	vec2 original_pos = pos_in_meters;
	vec2 original_vel = vel_in_meters_per_sec;

	auto acceleration = [this](vec2 pos, vec2 vel) {
		/* return ComputeSpringForce(pos, vel) / mass_in_grams; // Replace with your actual spring force logic */
		return GetAcceleration();
	};

	auto f_v = [&](real time, vec2 pos, vec2 vel) {
		return acceleration(pos, vel); // dv/dt = a(pos, vel)
	};

	auto f_x = [&](real time, vec2 pos, vec2 vel) {
		return vel; // dx/dt = vel
	};

	// Velocity RK4
	vec2 kv1 = f_v(0, original_pos, original_vel);
	vec2 kv2 = f_v(dt/2, original_pos + original_vel * dt/2, original_vel + kv1 * dt/2);
	vec2 kv3 = f_v(dt/2, original_pos + original_vel * dt/2, original_vel + kv2 * dt/2);
	vec2 kv4 = f_v(dt,   original_pos + original_vel * dt,   original_vel + kv3 * dt);
	vec2 dv = (dt / 6) * (kv1 + 2 * kv2 + 2 * kv3 + kv4);

	// Position RK4
	vec2 kx1 = f_x(0, original_pos, original_vel);
	vec2 kx2 = f_x(dt/2, original_pos + kx1 * dt/2, original_vel + kv1 * dt/2);
	vec2 kx3 = f_x(dt/2, original_pos + kx2 * dt/2, original_vel + kv2 * dt/2);
	vec2 kx4 = f_x(dt,   original_pos + kx3 * dt,   original_vel + kv3 * dt);
	vec2 dx = (dt / 6) * (kx1 + 2 * kx2 + 2 * kx3 + kx4);

	vel_in_meters_per_sec += dv;
	pos_in_meters += dx;

	ResetAcceleration(); // if still needed
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

		return;
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

		return;
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
		
		return;
	}
}
