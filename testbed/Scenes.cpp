#include <algorithm>
#include <cstdio>	// for "stderr" file path constant

#include "Scenes.hpp"
#include "Colour.hpp"
#include "GLFW/glfw3.h"
#include "JoePhys/Vec2.hpp"
#include "JoePhys/World.hpp"
#include "Renderer.hpp"
#include "SceneManager.hpp"
#include "Settings.hpp"

Scene::Scene()
{
	world = new World();
	selected_particle = nullptr;
	selected_particle_state = -1;

	
	// Will contain the particles added by the user
	Cluster* user_particles = new Cluster();
	world->clusters.push_back(user_particles);
}

Scene::~Scene()
{
	delete world;
}

void Scene::Render()
{
	// if there is no world, don't try to render
	if (world == nullptr)
		return;

	// loop over every cluster in the world
	for (int c = 0; c < world->clusters.size(); c++)
	{
		// loop over every particle in each cluster
		for (Particle* p : world->clusters[c]->particles)
		{
			// Render each particle
			int segments = settings.circle_res;
			real rad = p->radius_in_meters;
			vec2 pos = p->pos_in_meters;
			colour col;
			colour outline_col;
			if (p->is_static)
			{
				col = palette.colours[colours.static_particle];
				outline_col = palette.colours[colours.static_particle_outline];
			}
			else
			{
				col = palette.colours[colours.particle];
				outline_col = palette.colours[colours.particle_outline];
			}

			// Render particle
			renderer.AddSolidCircle(pos, rad, segments, col, outline_col);
		}
		// loop over every spring in each cluster
		if (settings.enable_springs)
			for (Spring* s : world->clusters[c]->springs)
			{
				// Render each spring if they are not torn
				vec2 posA = s->particleA->pos_in_meters;
				vec2 posB = s->particleB->pos_in_meters;
				colour col = palette.colours[colours.spring];
				if (!s->is_broken)
					renderer.AddLine(posA, posB, col);
			}
	}
	
	// Render a circle around selected particle
	colour outline_col(1.0f, 1.0f, 1.0f, 1.0f);
	if (selected_particle != nullptr)
		renderer.AddCircle(selected_particle->pos_in_meters, selected_particle->radius_in_meters * 1.2f, settings.circle_res, outline_col);
	
	// Render chunks
	if (settings.render_chunks)
		for (auto& [key, particles] : world->grid)
		{
			real x, y;
			world->GridKeyToChunkCoords(key, &x, &y);
	
			real left = x;
			real right = x + settings.chunk_scale;
			real top = y + settings.chunk_scale;
			real bottom = y;
			colour col(1.0f, 1.0f, 0.0f, 0.25f);
			colour out_col(0.3f, 0.3f, 0.0f, 1.00f);
			renderer.AddTriangle(vec2(left, top), vec2(right, top), vec2(left, bottom), col);
			renderer.AddTriangle(vec2(right, top), vec2(left, bottom), vec2(right, bottom), col);
			renderer.AddLine(vec2(left, top), vec2(right, top), out_col);
			renderer.AddLine(vec2(left, top), vec2(left, bottom), out_col);
			renderer.AddLine(vec2(right, bottom), vec2(right, top), out_col);
			renderer.AddLine(vec2(right, bottom), vec2(left, bottom), out_col);
		}
}


void Scene::ResetCamera()
{
	camera.center.Set(0.0f, 0.0f);
	camera.zoom = 1.0f;
}

void Scene::SetUpScene()
{
	return;
}

void Scene::SetUpSceneSettings()
{
	settings.Reset();
}

void Scene::Step()
{
	if (world != nullptr)
	{
		if (world->sub_steps != settings.sub_steps)
			world->sub_steps = settings.sub_steps;
		if (world->simulation_hertz != settings.simulation_hertz * settings.time_divisor)
			world->simulation_hertz = settings.simulation_hertz * settings.time_divisor;
		if (world->gravity != settings.gravity)
			world->gravity = settings.gravity;
		if (world->chunk_scale != settings.chunk_scale)
			world->chunk_scale = settings.chunk_scale;

		int flags = 0;
		if (!settings.enable_gravity)
			flags |= NO_GRAVITY;
		if (!settings.enable_drag)
			flags |= NO_DRAG;
		if (!settings.enable_particle_movement)
			flags |= NO_PARTICLE_MOVEMENT;
		if (!settings.enable_collision)
			flags |= NO_PARTICLE_COLLISIONS;
		if (!settings.enable_springs)
			flags |= NO_SPRING_FORCES;
		world->Step(flags);
	}
}

void Scene::UpdateSelectedParticle(vec2 cursor_pos)
{
	selected_particle = GetNearestParticle(camera.ScreenSpaceToWorldSpace(cursor_pos));
}

void Scene::AddStaticParticle(vec2 pos, real radius)
{
	if (world == nullptr)
		return;

	vec2 vel(0.0f, 0.0f);
	real elas = 0.5f;
	real mass = 1.0f;
	Particle* p = new Particle(pos, vel, elas, radius, mass, 1);
	world->clusters[0]->particles.push_back(p);
	return;
}

void Scene::AddRepulsionForce(vec2 pos, real radius, real strength)
{
	if (world == nullptr)
		return;

	// Render visualisation
	colour fill_col = palette.colours[colours.particle];
	fill_col.Set(fill_col.r*0.5f, fill_col.g*0.5f, fill_col.b*0.5f, fill_col.a*0.25f);
	colour outline_col = palette.colours[colours.particle_outline];
	renderer.AddSolidCircle(pos, radius, (int)radius * 5 + 10, fill_col, outline_col);

	// loop over every particle in every cluster and add repulsion force
	for (Cluster* c : world->clusters)
		for (Particle* p : c->particles)
		{

			// If particle is inside of the radius
			if (length(pos - p->pos_in_meters) <= radius) 
			{
				// Add repulsion force to every particle
				vec2 force = normalize(pos - p->pos_in_meters) * strength;
				p->Accelerate(-force);
			}
		}
}

void Scene::AddAttractionForce(vec2 pos, real radius, real strength)
{
	if (world == nullptr)
		return;

	// Render visualisation
	colour fill_col = palette.colours[colours.particle];
	fill_col.Set(fill_col.r*0.5f, fill_col.g*0.5f, fill_col.b*0.5f, fill_col.a*0.25f);
	colour outline_col = palette.colours[colours.particle_outline];
	renderer.AddSolidCircle(pos, radius, (int)radius * 5 + 10, fill_col, outline_col);

	// loop over every particle in every cluster and add repulsion force
	for (Cluster* c : world->clusters)
		for (Particle* p : c->particles)
		{

			// If particle is inside of the radius
			if (length(pos - p->pos_in_meters) <= radius) 
			{
				// Add attraction force to every particle
				vec2 force = normalize(pos - p->pos_in_meters) * strength;
				p->Accelerate(force);
			}
		}
}

Particle* Scene::GetNearestParticle(vec2 pos)
{
	if (world == nullptr)
		return nullptr;

	Particle* nearest = nullptr;

	// loop over every non static particle in every cluster
	for (Cluster* c : world->clusters)
		for (Particle* p : c->particles)
		{
			if (nearest == nullptr)
				nearest = p;

			if (lengthSquared(p->pos_in_meters - pos) < lengthSquared(nearest->pos_in_meters - pos))
				nearest = p;
		}
	
	return nearest;
}

void Scene::MoveParticle(Particle* part, vec2 pos)
{
	vec2 old_pos = part->pos_in_meters;

	// Reset and velocity or acceleration
	part->ResetAcceleration();
	part->vel_in_meters_per_sec.Set(0.0f, 0.0f);

	// Teleport particle to position
	part->pos_in_meters = pos;
}
