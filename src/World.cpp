#include <cstdio>	// for "stderr" file path constant

#include "JoePhys/World.hpp"

World::World(int simulation_hertz, vec2 gravity)
{
	this->simulation_hertz = simulation_hertz;
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
	UpdateParticlePositions();
	TemporaryConstrainToBox();
	ResolveParticleCollisions();
}

void World::UpdateParticlePositions()
{
	// TODO: Implement the runge-kutta method
	
	// TEMPORARY::EULER'S INTEGRATION TO MOVE PARTICLES
	double dt = 1.0f / simulation_hertz;
	for (int obj = 0; obj < PhysObjects.size(); obj++)
	{
		PhysObj* current_obj = PhysObjects[obj];
		for (int part = 0; part < current_obj->particles.size(); part++)
		{
			Particle* current_part = current_obj->particles[part];
			current_part->position += current_part->velocity * dt;
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
			const int box = 250;
			real rad = current_part->radius;
			vec2 pos = current_part->position;
			vec2 vel = current_part->velocity;
			
			if (pos.x + rad > box)
			{
				current_part->velocity = vec2(-vel.x, vel.y);
				current_part->position = vec2(box - rad, pos.y);
			}
			if (pos.y + rad > box)
			{
				current_part->velocity = vec2(vel.x, -vel.y);
				current_part->position = vec2(pos.x, box - rad);
			}
			if (pos.x - rad < -box)
			{
				current_part->velocity = vec2(-vel.x, vel.y);
				current_part->position = vec2(-box + rad, pos.y);
			}
			if (pos.y - rad < -box)
			{
				current_part->velocity = vec2(vel.x, -vel.y);
				current_part->position = vec2(pos.x, -box + rad);
			}
		}
	}
}

void World::ResolveParticleCollisions()
{
	// TODO: Collision resolution
	
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

					// Check for collision between particle A and B
					real dist = length(particleA->position - particleB->position);
					vec2 impactAxis = particleB->position - particleA->position;
					if (dist < particleA->radius + particleB->radius)
					{
						// Collision has occurred, we must now solve it

						real overlap = dist - (particleA->radius + particleB->radius);
						vec2 dir = impactAxis.GetNormalized();
						dir *= overlap * 0.5f;
						particleA->position += dir;
						particleB->position -= dir;

						real mSum = particleA->mass + particleB->mass;
						vec2 vDiff = particleB->velocity - particleA->velocity;

						// Particle A
						real numA = 2 * particleB->mass * dot(vDiff, impactAxis);
						real den = mSum * dist * dist;
						vec2 deltaVA = impactAxis * (numA / den);
						particleA->velocity += deltaVA;

						// Particle B
						vDiff *= -1;
						impactAxis *= -1;
						real numB = 2 * particleA->mass * dot(vDiff, impactAxis);
						vec2 deltaVB = impactAxis * (numB/ den);
						particleB->velocity += deltaVB;

					}
				}
		}
}
