#ifndef FLOPPY_QUADS_SCENE
#define FLOPPY_QUADS_SCENE

// TEMPORARY FOR "stderr"
#include <cstdio>

#include "../Scenes.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"
#include "JoePhys/Particle.hpp"
#include "JoePhys/Spring.hpp"

struct FloppyQuadsScene : public Scene
{
	// constructors & destructors
	void SetUpScene() override
	{
		SetUpSceneColours();
		world->Create(settings.simulation_hertz, settings.sub_steps, settings.gravity);
		double PI = 3.141592653589;

		// Create a bowl to catch quads
		// ----------------------------

		Cluster* bowl = new Cluster;

		// Loop over 
		int bowl_part_count = 10;
		vec2 pos(0.0f, 0.0f);
		for (int n = 0; n < bowl_part_count; n++)
		{
				real len = 6.0f;
				real radian = PI;
				real posx = cos(radian) * len;
				real posy = sin(radian) * len;
				posx += anchors->particles[x]->pos.x;
				posy += 4.0f;

				pos.Set(posx, posy);
		}
		

		// Add clusters to world
		world->clusters.push_back(bowl);
	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_DARK_YELLOW;
		colours.spring = Palette::JP_DARK_GRAY;
		colours.particle = Palette::JP_BLUE;
		colours.particle_outline = Palette::JP_DARK_GRAY;
		colours.static_particle = Palette::JP_DARK_BLUE;
		colours.static_particle_outline = Palette::JP_DARK_GRAY;
	}

	void SetUpSceneSettings() override
	{
		settings.Reset();

		settings.attraction_tool_strength = 25.0f;
		settings.repulsion_tool_strength = 25.0f;

		settings.gravity.Set(0.0f, -9.8f);
		settings.circle_res = 20;
		settings.chunk_scale = 1.7f;
		settings.sub_steps = 64;

		camera.center.Set(0.0f, 0.0f);
		camera.zoom = 2.0f;
	}
};

#endif
