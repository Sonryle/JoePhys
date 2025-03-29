#include <algorithm>
#include <cstdio>	// for "stderr" file path constant

#include "Scenes.hpp"
#include "Colour.hpp"
#include "JoePhys/Vec2.hpp"
#include "JoePhys/World.hpp"
#include "Renderer.hpp"
#include "Settings.hpp"

Scene::Scene()
{
	ResetCamera();
	world = new World();
	
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
				// Render each spring
				vec2 posA = s->particleA->pos_in_meters;
				vec2 posB = s->particleB->pos_in_meters;
				colour col = palette.colours[colours.spring];

				renderer.AddLine(posA, posB, col);
			}
	}
}


void Scene::ResetCamera()
{
	camera.center.Set(0.0f, 0.0f);
	camera.zoom = 1.0f;
}

void Scene::Step()
{
	if (world != nullptr)
	{
		if (world->sub_steps != settings.sub_steps)
			world->sub_steps = settings.sub_steps;
		if (world->simulation_hertz != settings.simulation_hertz * settings.time_divisor)
			world->simulation_hertz = settings.simulation_hertz * settings.time_divisor;

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

void Scene::AddRepulsionForce(vec2 pos, real amplitude)
{
	if (world == nullptr)
		return;
	
	// Render visualisation
	colour fill_col = palette.colours[colours.background];
	fill_col.Set(fill_col.r*0.5f, fill_col.g*0.5f, fill_col.b*0.5f, fill_col.a*0.25f);
	colour outline_col = palette.colours[colours.particle_outline];
	renderer.AddSolidCircle(pos, amplitude, (int)amplitude * 5, fill_col, outline_col);

	// loop over every particle in every cluster and add repulsion force
	for (Cluster* c : world->clusters)
		for (Particle* p : c->particles)
		{
			// Add repulsion force to every particle
			real dist = length(p->pos_in_meters - pos);
			real force = std::max(0.0f, amplitude - (dist));
			p->Accelerate(force * (p->pos_in_meters - pos));
		}
}

void Scene::AddAttractionForce(vec2 pos, real amplitude)
{
	if (world == nullptr)
		return;

	// Render visualisation
	colour fill_col = palette.colours[colours.particle];
	fill_col.Set(fill_col.r*0.5f, fill_col.g*0.5f, fill_col.b*0.5f, fill_col.a*0.25f);
	colour outline_col = palette.colours[colours.particle_outline];
	renderer.AddSolidCircle(pos, amplitude, (int)amplitude * 5, fill_col, outline_col);

	// loop over every particle in every cluster and add repulsion force
	for (Cluster* c : world->clusters)
		for (Particle* p : c->particles)
		{
			// Add repulsion force to every particle
			real dist = length(p->pos_in_meters - pos);
			real force = std::max(0.0f, amplitude - (dist));
			p->Accelerate(force * (pos - p->pos_in_meters));
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

	part->ResetAcceleration();
	part->vel_in_meters_per_sec = (pos - old_pos) * world->simulation_hertz;
	part->pos_in_meters = pos;
	return;
}
