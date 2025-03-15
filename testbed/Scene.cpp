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

	// loop over every particle in every physics object
	for (int c = 0; c < world->clusters.size(); c++)
		for (Particle* p : world->clusters[c]->particles)
		{
			int segments = settings.circle_res;
			real rad = p->radius;
			vec2 pos = p->position;
			colour col = palette.colours[settings.scene_colours.circles];
			colour outline_col = palette.colours[settings.scene_colours.circle_outlines];

			if (c == 0)
				col = palette.colours[Palette::JP_GREEN];
			if (c == 1)
				col = palette.colours[Palette::JP_RED];
	
			// Render particle
			renderer.AddSolidCircle(pos, rad, segments, col, outline_col);
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
