#include "JoePhys/Particle.hpp"

void Particle::Accelerate(vec2 force)
{
	if (!is_static)
		acc += force / mass;
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

void Particle::UpdatePosition(real dt)
{
	if (is_static)
		return;

	// Velocity with runge-kutta 2nd order
	vec2 kv1 = acc + spring_acc[0];
	vec2 kv2 = acc + spring_acc[1];
	vec2 dv = (dt / 2) * (kv1 + kv2);

	// Position with runge-kutta 2nd order
	vec2 kx1 = vel;
	vec2 kx2 = vel + dv;
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
	
	// if particle one is NOT static and particle two IS static
	if (!p1->is_static && p2->is_static)
	{
		// solve for particle one
		real m_ratio = 1+CoR;
		vec2 v_diff = p1->vel - p2->vel;
		vec2 p_diff = p1->pos - p2->pos;
		vec2 proj = project(v_diff, p_diff);
		p1->vel -= (proj * m_ratio);
		p2->vel.Set(0, 0);

		return;
	}
	// if particle one IS static and particle two is NOT static
	if (p1->is_static && !p2->is_static)
	{
		// solve for particle two
		real m_ratio = 1+CoR;
		vec2 v_diff = p2->vel - p1->vel;
		vec2 p_diff = p2->pos - p1->pos;
		vec2 proj = project(v_diff, p_diff);
		p1->vel.Set(0, 0);
		p2->vel -= (proj * m_ratio);

		return;
	}
	// if neither particles are static
	if (!p1->is_static && !p2->is_static)
	{
		// solve for particle one
		real m_ratio = ((1+CoR)*p2->mass) / (p1->mass + p2->mass);
		vec2 v_diff = p1->vel - p2->vel;
		vec2 p_diff = p1->pos - p2->pos;
		vec2 proj = project(v_diff, p_diff);
		vec2 dp1 = (proj * m_ratio);
		
		// solve for particle two
		m_ratio = ((1+CoR)*p1->mass) / (p1->mass + p2->mass);
		v_diff = p2->vel - p1->vel;
		p_diff = p2->pos - p1->pos;
		proj = project(v_diff, p_diff);
		vec2 dp2 = (proj * m_ratio);
		
		p1->vel -= dp1;
		p2->vel -= dp2;
		
		return;
	}
}
