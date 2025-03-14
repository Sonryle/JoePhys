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
	// loop through every Physics Object and delete it
	for (int n = 0; n < PhysObjects.size(); n++)
		delete PhysObjects[n];
}

void World::Step()
{
	real dt = 1.0f / simulation_hertz;

	for (int n = 0; n < sub_steps; n++)
	{
		ApplyGravityToParticles();
		UpdateParticlePositions(dt / sub_steps);
		ResolveParticleCollisions();
		TemporaryConstrainToBox();
		ResolveParticleCollisions();
	}
}

void World::ApplyGravityToParticles()
{
	// loop over every particle and add gravity to its acceleration
	for (int obj = 0; obj < PhysObjects.size(); obj++)
	{
		PhysObj* current_obj = PhysObjects[obj];
		for (int part = 0; part < current_obj->particles.size(); part++)
		{
			current_obj->particles[part]->acceleration += gravity;
		}
	}
}

// Move particles forwards along their velocities
void World::UpdateParticlePositions(real dt)
{

	for (int obj = 0; obj < PhysObjects.size(); obj++)
	{
		PhysObj* phys_object = PhysObjects[obj];
		for (int part = 0; part < phys_object->particles.size(); part++)
		{
			Particle* particle = phys_object->particles[part];
			// since acceleration remains constant over time step, velocity
			// can be updated using euler's integration
			particle->velocity += particle->acceleration * dt;

			vec2 v = particle->velocity;
			vec2 a = particle->acceleration;

			vec2 k1 = v;
			vec2 k2 = v + (a * dt / 2.0f);
			vec2 k3 = v + (a * dt / 2.0f);
			vec2 k4 = v + (a * dt);

			particle->position += ((k1 + 2.0f*k2 + 2.0f*k3 + k4) / 6.0f) * dt;
			particle->acceleration.Set(0.0f, 0.0f);
		}
	}
}

void World::TemporaryConstrainToBox()
{
	// loop over every particle and constrain them
	for (int obj = 0; obj < PhysObjects.size(); obj++)
	{
		PhysObj* current_obj = PhysObjects[obj];
		for (int part = 0; part < current_obj->particles.size(); part++)
		{
			Particle* current_part = current_obj->particles[part];

			// check for collisions with walls of box
			const int boxy = 250;
			const int boxx = 500;
			real elas = current_part->elasticity;
			/* real elas = 1.0f; */
			real rad = current_part->radius;
			vec2 pos = current_part->position;
			vec2 vel = current_part->velocity;
			
			if (pos.x + rad > boxx)
			{
				current_part->velocity.x *= -1 * elas;
				current_part->position.x = boxx - rad;
			}
			if (pos.y + rad > boxy)
			{
				current_part->velocity.y *= -1 * elas;
				current_part->position.y = boxy - rad;
			}
			if (pos.x - rad < -boxx)
			{
				current_part->velocity.x *= -1 * elas;
				current_part->position.x = -boxx + rad;
			}
			if (pos.y - rad < -boxy)
			{
				current_part->velocity.y *= -1 * elas;
				current_part->position.y = -boxy + rad;
			}
		}
	}
}

void World::ResolveParticleCollisions()
{
	// Credit for Particle Collision Resolution goes to "The Coding Train"
	// https://www.youtube.com/watch?v=dJNFPv9Mj-Y&t=1421s
	
	// loop over every particle in every physObj and compair it to every other particle in every other physObj
	for (int objA = 0; objA < PhysObjects.size(); objA++)
		for (int partA = 0; partA < PhysObjects[objA]->particles.size(); partA++)
		{
			Particle* particleA = PhysObjects[objA]->particles[partA];

			// compair particle A to every other particle in every other physObj
			// (except physObjects and particles which have already been looped over)
			for (int objB = objA; objB < PhysObjects.size(); objB++)
				for (int partB = partA + 1; partB < PhysObjects[objB]->particles.size(); partB++)
				{
					Particle* particleB = PhysObjects[objB]->particles[partB];

					// If dist between particles is less than their radii then they have collided
					real dist = length(particleA->position - particleB->position);
					if (dist < particleA->radius + particleB->radius)
					{
						// difference in position between particles
						vec2 pos_diff = particleB->position - particleA->position;

						// Move particles away from eachother along their impact axis
						vec2 dir = pos_diff.GetNormalized();
						real overlap = dist - (particleA->radius + particleB->radius);
						dir *= overlap * 0.5f;
						particleA->position += dir;
						particleB->position -= dir;

						// Solve for new velocities

						// Particle A
						real mass_sum = particleA->mass + particleB->mass;
						vec2 vel_diff = particleB->velocity - particleA->velocity;
						real denominator = mass_sum * dist * dist;
						real numerator = 2 * particleB->mass * dot(vel_diff, pos_diff);
						vec2 delta_vel = pos_diff * (numerator / denominator);
						particleA->velocity += delta_vel * particleA->elasticity;

						// Particle B
						vel_diff *= -1;
						pos_diff *= -1;
						numerator = 2 * particleA->mass * dot(vel_diff, pos_diff);
						delta_vel = pos_diff * (numerator/ denominator);
						particleB->velocity += delta_vel * particleB->elasticity;

					}
				}
		}
}
