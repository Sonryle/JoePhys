#include <cstdio>	// for "stderr" file path constant

#include "JoePhys/World.hpp"
#include "JoePhys/Vec2.hpp"

World::~World()
{
	// loop through every Cluster and delete it
	for (Cluster* n : clusters)
		delete n;
}

void World::Create(int simulation_hertz, int sub_steps, vec2 gravity)
{
	this->simulation_hertz = simulation_hertz;
	this->sub_steps = sub_steps;
	this->gravity = gravity;
}

void World::Step(int flags)
{
	real dt = 1.0f / simulation_hertz;

	for (int n = 0; n < sub_steps; n++)
	{
		if (!(flags & NO_GRAVITY))
			ApplyGravityToParticles();
		if (!(flags & NO_DRAG))
			ApplyDragToParticles();

		if (!(flags & NO_PARTICLE_MOVEMENT))
			UpdateParticlePositions(dt / sub_steps);
		if (!(flags & NO_SPRING_FORCES))
			UpdateSprings(dt / sub_steps);

		if (!(flags & NO_PARTICLE_COLLISIONS))
			ResolveAllCollisions();
	}
}

void World::ApplyGravityToParticles()
{
	// loop over every particle in every cluster and add gravity to its acceleration
	for (Cluster* c : clusters)
		for (Particle* p : c->particles)
			p->Accelerate(p->mass_in_grams * gravity);
}

void World::ApplyDragToParticles()
{
	double PI = 3.14159265359;
	// loop over every particle in every cluster and apply air resistance
	for (Cluster* c : clusters)
		for (Particle* pt : c->particles)
			pt->ApplyDrag(0.0047f);	// average dampening of air is 0.0047
}

// Move particles forwards along their velocities
void World::UpdateParticlePositions(real dt)
{
	// Loop over every particle in every cluster and update its position
	for (Cluster* c : clusters)
		for (Particle* p : c->particles)
			p->UpdatePosition(dt);
}

void World::ResolveAllCollisions()
{
	// look over every particle in every cluster
	for (int c1 = 0; c1 < clusters.size(); c1++)
		for (int p1 = 0; p1 < clusters[c1]->particles.size(); p1++)
			// compair it to every other particle in every other cluster
			for (int c2 = c1; c2 < clusters.size(); c2++)
				for (int p2 = (c2 == c1)? p1 + 1 : 0; p2 < clusters[c2]->particles.size(); p2++)
				{
					// Solve collision between the two particles only if they collide

					Particle* particle_one = clusters[c1]->particles[p1];
					Particle* particle_two = clusters[c2]->particles[p2];
					real dist = length(particle_one->pos_in_meters - particle_two->pos_in_meters);

					// If both particles are static, skip
					if (particle_one->is_static && particle_two->is_static)
						continue;

					// If dist between particles is less than their radii then they have collided
					if (dist < particle_one->radius_in_meters + particle_two->radius_in_meters)
						particle_one->ResolveCollision(particle_two);
				}
}

void World::UpdateSprings(real dt)
{
	// loop over every spring in every cluster and update them
	for (Cluster* c : clusters)
		for (Spring* s : c->springs)
			s->Update();
}
