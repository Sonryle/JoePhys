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

		UpdateGrid();
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

					// If both particles are static, skip
					if (particle_one->is_static && particle_two->is_static)
						continue;

					// If dist between particles is less than their radii then they have collided
					real dist = length(particle_one->pos_in_meters - particle_two->pos_in_meters);
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

void World::PositionToChunkCoords(vec2 pos, int32_t* chunk_x, int32_t* chunk_y)
{
	*chunk_x = (int)(pos.x / chunk_scale);
	*chunk_y = (int)(pos.y / chunk_scale);
}

void World::ChunkCoordsToGridKey(int64_t* key, int32_t chunk_x, int32_t chunk_y)
{
	// Get key for our chunk
	*key = (int64_t)chunk_x;
	*key <<= 32;
	*key |= (uint32_t)chunk_y;
}

void World::GridKeyToChunkCoords(int64_t key, int32_t* chunk_x, int32_t* chunk_y)
{
	*chunk_x = (int32_t)(key >> 32) * chunk_scale;
	*chunk_y = (int32_t)(key)* chunk_scale;
}

void World::UpdateGrid()
{
	// Clear particles from grid which arent in that chunk anymore
	// -----------------------------------------------------------

	std::vector<int64_t> chunks_to_delete;
	for (auto& [key, particles] : grid)
	{
		/* // Decode key into x and y positions */
		int32_t chunk_x;
		int32_t chunk_y;
		GridKeyToChunkCoords(key, &chunk_x, &chunk_y);

		// If particles arent in the chunk that they
		// used to be in, remove them from the list
		std::vector<Particle*> particles_to_delete;
		for (Particle* p : particles)
		{
			real top = p->pos_in_meters.y + p->radius_in_meters;
			real bottom = p->pos_in_meters.y - p->radius_in_meters;
			real right = p->pos_in_meters.x + p->radius_in_meters;
			real left = p->pos_in_meters.x - p->radius_in_meters;
			if (left < 0)
				left -= chunk_scale;
			if (bottom < 0)
				bottom -= chunk_scale;

			int32_t top_chunk;
			int32_t bottom_chunk;
			int32_t left_chunk;
			int32_t right_chunk;
			PositionToChunkCoords(vec2(left, top), &left_chunk, &top_chunk);
			PositionToChunkCoords(vec2(right, bottom), &right_chunk, &bottom_chunk);

			particles_to_delete.push_back(p);
			/* if (chunk_x <= left_chunk && chunk_x >= right_chunk) */
				/* if (chunk_y >= bottom_chunk && chunk_y <= top_chunk) */
				/* 	particles_to_delete.pop_back(); */
		}

		for (Particle* p : particles_to_delete)
			particles.erase(p);

		// If chunk is empty, delete it
		if (particles.empty())
			chunks_to_delete.push_back(key);
	}

	// Delete grids
	for (int64_t key : chunks_to_delete)
		grid.erase(key);

	// Add all particles to grid
	// -------------------------
	
	// Loop over particles and add them to their chunks (if not already added)
	for (Cluster* c : clusters)
		for (Particle* p : c->particles)
		{
			// Find the top, bottom, left and right of each particle
			real top = p->pos_in_meters.y + p->radius_in_meters;
			real bottom = p->pos_in_meters.y - p->radius_in_meters;
			real right = p->pos_in_meters.x + p->radius_in_meters;
			real left = p->pos_in_meters.x - p->radius_in_meters;
			if (left < 0)
			{
				left -= chunk_scale;
				right -= chunk_scale;
			}
			if (bottom < 0)
			{
				bottom -= chunk_scale;
				top -= chunk_scale;
			}
			
			int32_t top_chunk;
			int32_t bottom_chunk;
			int32_t right_chunk;
			int32_t left_chunk;
			PositionToChunkCoords(vec2(left, top), &left_chunk, &top_chunk);
			PositionToChunkCoords(vec2(right, bottom), &right_chunk, &bottom_chunk);

			// Add particles to those extra chunks
			for (real x = left_chunk; x <= right_chunk; x++)
			{
				for (real y = bottom_chunk; y <= top_chunk; y++)
				{
					int64_t key;
					ChunkCoordsToGridKey(&key, x, y);
			
					// Add particle if its not already in the set
					if (std::find(grid[key].begin(), grid[key].end(), p) == grid[key].end())
						grid[key].insert(p);
				}
			}
		}
}
