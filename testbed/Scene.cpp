#include <cstdio>	// for "stderr" file path constant

#include "Scene.hpp"
#include "Colour.hpp"
#include "Renderer.hpp"
#include "Settings.hpp"

Scene::Scene()
{
	ResetCamera();
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
			real rad = p->radius;
			vec2 pos = p->position;
			colour col;
			colour outline_col;
			if (p->mass == 0.0f)
			{
				col = palette.colours[settings.scene_colours.static_particle];
				outline_col = palette.colours[settings.scene_colours.static_particle_outline];
			}
			else
			{
				col = palette.colours[settings.scene_colours.particle];
				outline_col = palette.colours[settings.scene_colours.particle_outline];
			}

			// Render particle
			renderer.AddSolidCircle(pos, rad, segments, col, outline_col);
		}
		// loop over every spring in each cluster
		for (Spring* s : world->clusters[c]->springs)
		{
			// Render each spring
			vec2 posA = s->particleA->position;
			vec2 posB = s->particleB->position;
			colour col = palette.colours[settings.scene_colours.spring];

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
		if (world->simulation_hertz != settings.simulation_hertz)
			world->simulation_hertz = settings.simulation_hertz;
		world->Step();
	}
}
