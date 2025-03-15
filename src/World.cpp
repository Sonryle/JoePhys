#include <cstdio>	// for "stderr" file path constant

#include "JoePhys/World.hpp"

World::World(int simulation_hertz, int sub_steps, vec2 gravity)
{
	this->simulation_hertz = simulation_hertz;
	this->sub_steps = sub_steps;
	this->gravity = gravity;
}

World::~World()
{
	// loop through every Cluster and delete it
	for (Cluster* n : clusters)
		delete n;
}

void World::Step()
{
	real dt = 1.0f / simulation_hertz;

	for (int n = 0; n < sub_steps; n++)
	{
		ApplyGravityToParticles();
		UpdateParticlePositions(dt / sub_steps);
		ResolveAllCollisions();
		TemporaryConstrainToBox();
		ResolveAllCollisions();
	}
}

void World::ApplyGravityToParticles()
{
	// loop over every particle in every cluster and add gravity to its acceleration
	for (Cluster* c : clusters)
		for (Particle* p : c->particles)
			p->acceleration += gravity;
}

// Move particles forwards along their velocities
void World::UpdateParticlePositions(real dt)
{
	// Loop over every particle in every cluster and update its position
	for (Cluster* c : clusters)
		for (Particle* p : c->particles)
		{
			// since acceleration remains constant over time step, velocity
			// can be updated using euler's integration
			p->velocity += p->acceleration * dt;
	
			vec2 v = p->velocity;
			vec2 a = p->acceleration;
	
			vec2 k1 = v;
			vec2 k2 = v + (a * dt / 2.0f);
			vec2 k3 = v + (a * dt / 2.0f);
			vec2 k4 = v + (a * dt);
	
			p->position += ((k1 + 2.0f*k2 + 2.0f*k3 + k4) / 6.0f) * dt;
			p->acceleration.Set(0.0f, 0.0f);
		}
}

void World::TemporaryConstrainToBox()
{
	// loop over every particle in every cluster and constrain them
	for (Cluster* c : clusters)
		for (Particle* p : c->particles)
		{
			// check for collisions with walls of box
			const int boxy = 250;
			const int boxx = 500;
			real elas = p->elasticity;
			/* real elas = 1.0f; */
			real rad = p->radius;
			vec2 pos = p->position;
			vec2 vel = p->velocity;
			
			if (pos.x + rad > boxx)
			{
				p->velocity.x *= -1 * elas;
				p->position.x = boxx - rad;
			}
			if (pos.y + rad > boxy)
			{
				p->velocity.y *= -1 * elas;
				p->position.y = boxy - rad;
			}
			if (pos.x - rad < -boxx)
			{
				p->velocity.x *= -1 * elas;
				p->position.x = -boxx + rad;
			}
			if (pos.y - rad < -boxy)
			{
				p->velocity.y *= -1 * elas;
				p->position.y = -boxy + rad;
			}
		}
}

// Solves collision between two particles, pA & pB
void ResolveCollision(Particle* pA, Particle* pB)
{
	// Credit for Particle Collision Resolution goes to "The Coding Train"
	// https://www.youtube.com/watch?v=dJNFPv9Mj-Y&t=1421s
	
	// If dist between particles is less than their radii then they have collided
	real dist = length(pA->position - pB->position);
	if (dist < pA->radius + pB->radius)
	{
		// difference in position between particles
		vec2 pos_diff = pB->position - pA->position;

		// Move particles away from eachother along their impact axis
		vec2 dir = pos_diff.GetNormalized();
		real overlap = dist - (pA->radius + pB->radius);
		dir *= overlap * 0.5f;
		pA->position += dir;
		pB->position -= dir;

		// Solve for new velocities

		// Particle A
		real mass_sum = pA->mass + pB->mass;
		vec2 vel_diff = pB->velocity - pA->velocity;
		real denominator = mass_sum * dist * dist;
		real numerator = 2 * pB->mass * dot(vel_diff, pos_diff);
		vec2 delta_vel = pos_diff * (numerator / denominator);
		pA->velocity += delta_vel * pA->elasticity;

		// Particle B
		vel_diff *= -1;
		pos_diff *= -1;
		numerator = 2 * pA->mass * dot(vel_diff, pos_diff);
		delta_vel = pos_diff * (numerator/ denominator);
		pB->velocity += delta_vel * pB->elasticity;

	}
}


void World::ResolveAllCollisions()
{
	// loop over every particle in every cluster and compair it to every other particle
	// in every other cluster
	
	// c1 is the location of the cluster which we are looking at
	// p1 is the location of the particle which we are looking at inside of c1
	//
	// c2 is the location of the second cluster (we compair all of the particles inside c1 to all of the particles inside of c1)
	// p2 is the location of the second particle inside of c2

	for (int c1 = 0; c1 < clusters.size(); c1++)
		for (int p1 = 0; p1 < clusters[c1]->particles.size(); p1++)
			for (int c2 = c1; c2 < clusters.size(); c2++)
				for (int p2 = (c2 == c1)? p1 + 1 : 0; p2 < clusters[c2]->particles.size(); p2++)
					ResolveCollision(clusters[c1]->particles[p1], clusters[c2]->particles[p2]);
}
