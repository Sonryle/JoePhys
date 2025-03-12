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

	// loop over every physics object in the world
	for (int obj = 0; obj < world->PhysObjects.size(); obj++)
	{
		// Create a copy of the PhysObject that we're looking at
		PhysObj* current_obj = world->PhysObjects[obj];

		// loop over every particle in the physics objectttt
		for (int part = 0; part < current_obj->particles.size(); part++)
		{
			// Create a copy of the particle we're looking at
			Particle* current_part = current_obj->particles[part];

			int segments = settings.circle_res;
			real rad = current_part->radius;
			vec2 pos = current_part->position;
			colour col = palette.colours[settings.scene_colours.circles];
			colour outline_col = palette.colours[settings.scene_colours.circle_outlines];

			// Render particle
			renderer.AddSolidCircle(pos, rad, segments, col, outline_col);
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
