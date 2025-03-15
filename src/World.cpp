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
	}
}

void World::ApplyGravityToParticles()
{
	// loop over every particle in every cluster and add gravity to its acceleration
	for (Cluster* c : clusters)
		for (Particle* p : c->particles)
			if (p->mass != 0.0f)	// if particle isnt static
				p->acceleration += gravity;
}

// Move particles forwards along their velocities
void World::UpdateParticlePositions(real dt)
{
	// Loop over every particle in every cluster and update its position
	for (Cluster* c : clusters)
		for (Particle* p : c->particles)
		{
			// if particle is static, skip it
			if (p->mass == 0.0f)
				continue;

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
		// ----------------------------------------------------------

		vec2 dir = pos_diff.GetNormalized();
		real overlap = dist - (pA->radius + pB->radius);
		dir *= overlap * 0.5f;
		// if either particles are static, move opposite particle accordingly
		if (pA->mass != 0.0f && pB->mass != 0.0f)
		{
			pA->position += dir;
			pB->position -= dir;
		}
		else if (pA->mass == 0.0f && pB->mass != 0.0f)
			pB->position -= dir * 2.0f;
		else if (pA->mass != 0.0f && pB->mass == 0.0f)
			pA->position += dir * 2.0f;

		// Solve for new velocities
		// ------------------------

		real pAMass = pA->mass;
		real pBMass = pB->mass;

		// change mass to be insanely high if object is static
		// so that most of the energy is kept by the object
		// colliding with it (i know its not the best solution)
		
		if (pA->mass == 0.0f)
			pAMass = 100000000000000.0f;
		if (pB->mass == 0.0f)
			pBMass = 100000000000000.0f;

		// Particle A

		real mass_sum;
		vec2 vel_diff;
		real denominator;
		real numerator;
		vec2 delta_vel;

		if (pA->mass != 0.0f)
		{
			mass_sum = pAMass + pBMass;
			vel_diff = pB->velocity - pA->velocity;
			denominator = mass_sum * dist * dist;
			numerator = 2 * pBMass * dot(vel_diff, pos_diff);
			delta_vel = pos_diff * (numerator / denominator);
			pA->velocity += delta_vel * pA->elasticity;
		}

		// Particle B
		if (pB->mass != 0.0f)
		{
			vel_diff *= -1;
			pos_diff *= -1;
			numerator = 2 * pAMass * dot(vel_diff, pos_diff);
			delta_vel = pos_diff * (numerator/ denominator);
			pB->velocity += delta_vel * pB->elasticity;
		}

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
