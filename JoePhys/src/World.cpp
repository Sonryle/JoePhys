#include <cstdio>	// for "stderr" file path constant

#include <JoePhys/World.hpp>
#include <JoePhys/Vec2.hpp>

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
		if (!(flags & NO_SPRING_FORCES))
			UpdateSprings(dt / sub_steps);
		if (!(flags & NO_PARTICLE_MOVEMENT))
			UpdateParticlePositions(dt / sub_steps);

		if (!(flags & NO_PARTICLE_COLLISIONS))
		{
			UpdateChunks();
			ResolveAllCollisions();
		}

	}
}

void World::ApplyGravityToParticles()
{
	// loop over every particle in every cluster and add gravity to its acceleration
	for (Cluster* c : clusters)
		for (Particle* p : c->particles)
			p->Accelerate(p->mass * gravity);
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
	// look over every chunk and compair the particles inside of them
	for (auto& [key, particles] : chunks)
		for (int p1 = 0; p1 < particles.size(); p1++)
			for (int p2 = p1+1; p2 < particles.size(); p2++)
			{
				// Solve collision between the two particles only if they collide
				Particle* part1 = particles[p1];
				Particle* part2 = particles[p2];

				if ((part1 == part2) || (part1->is_static && part2->is_static))
					continue;

				// If dist between particles is less than their radii then they have collided
				real dist_squared = lengthSquared(part1->pos - part2->pos);
				real min_dist = part1->radius + part2->radius;

				if (dist_squared < min_dist * min_dist)
					part1->ResolveCollision(part2);
			}
}

void World::UpdateSprings(real dt)
{
	// loop over every spring in every cluster and update them
	for (Cluster* c : clusters)
		for (Spring* s : c->springs)
			s->Update(dt);
}

void World::PositionToChunkCoords(vec2 pos, int32_t* chunk_x, int32_t* chunk_y)
{
	*chunk_x = (int32_t)(pos.x / chunk_scale);
	*chunk_y = (int32_t)(pos.y / chunk_scale);
}

int64_t World::ChunkCoordsToHashKey(int32_t chunk_x, int32_t chunk_y)
{
	// Get key for our chunk
	int64_t key = (int64_t)chunk_x;
	key <<= 32;
	key |= (uint32_t)chunk_y;
	return key;
}

void World::HashKeyToChunkCoords(int64_t key, real* chunk_x, real* chunk_y)
{
	*chunk_x = (int32_t)(key >> 32) * chunk_scale;
	*chunk_y = (int32_t)(key)* chunk_scale;
}

void World::UpdateChunks()
{
	chunks.clear();

	// Add all particles to grid
	// -------------------------
	
	// Loop over particles and add them to their chunks (if not already added)
	for (Cluster* c : clusters)
		for (Particle* p : c->particles)
		{
			// Find the top, bottom, left and right of each particle
			real top = p->pos.y + p->radius;
			real bottom = p->pos.y - p->radius;
			real right = p->pos.x + p->radius;
			real left = p->pos.x - p->radius;
			if (left < 0) left -= chunk_scale;
			if (right < 0) right -= chunk_scale;
			if (bottom < 0) bottom -= chunk_scale;
			if (top < 0) top -= chunk_scale;
			
			int32_t top_chunk;
			int32_t bottom_chunk;
			int32_t right_chunk;
			int32_t left_chunk;
			PositionToChunkCoords(vec2(left, top), &left_chunk, &top_chunk);
			PositionToChunkCoords(vec2(right, bottom), &right_chunk, &bottom_chunk);

			// Add particles to those extra chunks
			for (real x = left_chunk; x <= right_chunk; x++)
				for (real y = bottom_chunk; y <= top_chunk; y++)
					chunks[ChunkCoordsToHashKey(x, y)].push_back(p);
		}
}
