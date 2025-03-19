#include <cstdio>	// for "stderr" file path constant

#include "JoePhys/World.hpp"
#include "JoePhys/Vec2.hpp"

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
		UpdateSprings(dt / sub_steps);
		ResolveAllCollisions();
	}
}

void World::ApplyGravityToParticles()
{
	// loop over every particle in every cluster and add gravity to its acceleration
	for (Cluster* c : clusters)
		for (Particle* p : c->particles)
			if (p->mass_in_grams != 0.0f)	// if particle isnt static
				p->Accelerate(p->mass_in_grams * gravity);
}

// Move particles forwards along their velocities
void World::UpdateParticlePositions(real dt)
{
	// Loop over every particle in every cluster and update its position
	for (Cluster* c : clusters)
		for (Particle* p : c->particles)
		{
			// if particle is static, skip it
			if (p->mass_in_grams == 0.0f)
				continue;

			// since acceleration remains constant over time step, velocity
			// can be updated using euler's integration
			p->vel_in_meters_per_sec += p->GetAcceleration() * dt;
	
			vec2 v = p->vel_in_meters_per_sec;
			vec2 a = p->GetAcceleration();
	
			vec2 k1 = v;
			vec2 k2 = v + (a * dt / 2.0f);
			vec2 k3 = v + (a * dt / 2.0f);
			vec2 k4 = v + (a * dt);
	
			p->pos_in_meters += ((k1 + 2.0f*k2 + 2.0f*k3 + k4) / 6.0f) * dt;
			p->ResetAcceleration();
		}
}

// Solves collision between two particles, pA & pB
void ResolveCollision(Particle* pA, Particle* pB)
{
	// Credit for Particle Collision Resolution goes to "The Coding Train"
	// https://www.youtube.com/watch?v=dJNFPv9Mj-Y&t=1421s
	
	// If dist between particles is less than their radii then they have collided
	real dist = length(pA->pos_in_meters - pB->pos_in_meters);
	if (dist < pA->radius_in_meters + pB->radius_in_meters)
	{
		// difference in position between particles
		vec2 pos_diff = pB->pos_in_meters - pA->pos_in_meters;

		// Move particles away from eachother along their impact axis
		// ----------------------------------------------------------

		vec2 dir = pos_diff.GetNormalized();
		real overlap = dist - (pA->radius_in_meters + pB->radius_in_meters);
		dir *= overlap * 0.5f;
		// if either particles are static, move opposite particle accordingly
		if (pA->mass_in_grams != 0.0f && pB->mass_in_grams != 0.0f)
		{
			pA->pos_in_meters += dir;
			pB->pos_in_meters -= dir;
		}
		else if (pA->mass_in_grams == 0.0f && pB->mass_in_grams != 0.0f)
			pB->pos_in_meters -= dir * 2.0f;
		else if (pA->mass_in_grams != 0.0f && pB->mass_in_grams == 0.0f)
			pA->pos_in_meters += dir * 2.0f;

		// Solve for new velocities
		// ------------------------

		real pAMass = pA->mass_in_grams;
		real pBMass = pB->mass_in_grams;

		// change mass to be insanely high if object is static
		// so that most of the energy is kept by the object
		// colliding with it (i know its not the best solution)
		
		if (pA->mass_in_grams == 0.0f)
			pAMass = 100000000000000.0f;
		if (pB->mass_in_grams == 0.0f)
			pBMass = 100000000000000.0f;

		// Particle A

		real mass_sum;
		vec2 vel_diff;
		real denominator;
		real numerator;
		vec2 delta_vel;

		mass_sum = pAMass + pBMass;
		vel_diff = pB->vel_in_meters_per_sec - pA->vel_in_meters_per_sec;
		denominator = mass_sum * dist * dist;
		numerator = 2 * pBMass * dot(vel_diff, pos_diff);
		delta_vel = pos_diff * (numerator / denominator);
		if (pA->mass_in_grams != 0.0f)
			pA->vel_in_meters_per_sec += delta_vel * pA->elasticity;

		// Particle B
		vel_diff *= -1;
		pos_diff *= -1;
		numerator = 2 * pAMass * dot(vel_diff, pos_diff);
		delta_vel = pos_diff * (numerator/ denominator);
		if (pB->mass_in_grams != 0.0f)
			pB->vel_in_meters_per_sec+= delta_vel * pB->elasticity;

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

void World::UpdateSprings(real dt)
{
	// loop over every spring and move particles away from eachother by spring length
	for (Cluster* c : clusters)
		for (Spring* s : c->springs)
		{
			vec2 dp = s->particleA->pos_in_meters - s->particleB->pos_in_meters;
			real dist = length(dp);
			real diff = s->resting_length - dist;
			vec2 dn = normalize(dp);

			real k = s->stiffness;
			vec2 x = dn * diff;
			vec2 force = k * x;

			s->particleA->Accelerate(force);
			s->particleB->Accelerate(-force);
		}
	return;
}
