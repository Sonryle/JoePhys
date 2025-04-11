#include "JoePhys/Particle.hpp"

void Particle::Accelerate(vec2 force)
{
	if (!is_static)
		acc += force / mass;
}

void Particle::Accelerate(int runge_kutta_sample, vec2 force)
{
	if (!is_static)
	{
		if (runge_kutta_sample == 1)
			spring_acc[0] += force / mass;
		if (runge_kutta_sample == 2)
			spring_acc[1] += force / mass;
	}
}

vec2 Particle::GetAcceleration()
{
	return acc;
}

vec2 Particle::GetSpringAcceleration(int runge_kutta_sample)
{
	return spring_acc[runge_kutta_sample-1];
}

void Particle::ResetAcceleration()
{
	acc.Set(0.0f, 0.0f);
	spring_acc[0].Set(0.0f, 0.0f);
	spring_acc[1].Set(0.0f, 0.0f);
}

void Particle::ApplyDrag(real dampening)
{
		if (is_static || length(vel) < 0.0001f)
			return;

		real vel_magnitude = length(vel);
		real drag_magnitude = vel_magnitude * vel_magnitude * dampening;
		vec2 drag_force = normalize(vel) * -drag_magnitude;

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

	auto f_v = [&](int runge_kutta_sample)
	{
		return GetAcceleration() + GetSpringAcceleration(runge_kutta_sample);
	};

	auto f_x = [&](real time, vec2 pos, vec2 vel)
	{
		return vel;
	};

	// Velocity RK4
	vec2 kv1 = f_v(1);
	vec2 kv2 = f_v(2);
	vec2 dv = (dt / 2) * (kv1 + kv2);

	// Position RK4
	vec2 kx1 = f_x(0, pos, vel);
	vec2 kx2 = f_x(dt, pos + kx1 * dt, vel + kv2 * dt);
	vec2 dx = (dt / 2) * (kx1 + kx2);

	vel += dv;
	pos += dx;

	ResetAcceleration();
}

// Solves collision between itself and another particle
void Particle::ResolveCollision(Particle* other_particle)
{
	Particle* p1 = this;
	Particle* p2 = other_particle;

	// Move particles away from eachother along their impact axis
	// ----------------------------------------------------------
	
	vec2 impact_axis = normalize(p1->pos - p2->pos);
	real dist_between_particles = length(p1->pos - p2->pos);
	real overlap = (p1->radius + p2->radius) - dist_between_particles;

	// consider whether each particle is static or dynamic.
	if ( p1->is_static && !p2->is_static)
		p2->pos -= impact_axis * overlap;
	if (!p1->is_static &&  p2->is_static)
		p1->pos += impact_axis * overlap;
	if (!p1->is_static && !p2->is_static)
	{
		p1->pos += impact_axis * overlap * 0.5f;
		p2->pos -= impact_axis * overlap * 0.5f;
	}


	// Solve for new velocities
	// -----------------------

	real CoR = (p1->elasticity + p2->elasticity) * 0.5f;	// Coefficient of Restitution
	vec2 p1_v = p1->vel;
	vec2 p2_v = p2->vel;
	vec2 p1_p = p1->pos;
	vec2 p2_p = p2->pos;
	real p1_m = p1->mass;
	real p2_m = p2->mass;
	
	// if particle one is NOT static and particle two IS static
	if (!p1->is_static && p2->is_static)
	{
		// solve for particle one
		real m_ratio = 1+CoR;
		vec2 v_diff = p1_v - p2_v;
		vec2 p_diff = p1_p - p2_p;
		vec2 proj = project(v_diff, p_diff);
		p1->vel -= (proj * m_ratio);

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
		p2->vel -= (proj * m_ratio);

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
		p1->vel -= (proj * m_ratio);
		
		// solve for particle two
		m_ratio = ((1+CoR)*p1_m) / (p1_m + p2_m);
		v_diff = p2_v - p1_v;
		p_diff = p2_p - p1_p;
		proj = project(v_diff, p_diff);
		p2->vel -= (proj * m_ratio);
		
		return;
	}
}
