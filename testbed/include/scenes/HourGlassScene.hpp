#ifndef HOUR_GLASS_SCENE
#define HOUR_GLASS_SCENE

// TEMPORARY FOR "stderr"
#include <cstdio>

#include "../Scenes.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"
#include "JoePhys/Particle.hpp"
#include "JoePhys/Spring.hpp"

struct HourGlassScene : public Scene
{
	// constructors & destructors
	void SetUpScene() override
	{
		SetUpSceneColours();
		world->Create(settings.simulation_hertz, settings.sub_steps, settings.gravity);
		double PI = 3.141592653589;

		// Create an hour glass to catch particles
		// ----------------------------

		Cluster* bowl = new Cluster;
		int bowl_part_count = 80;
		real radian = 0;
		real radius = 0.2f;
		real mass = PI * radius * radius;
		real len = 3;
		for (int n = 0; n < bowl_part_count + 1; n++)
		{
			real x = cos(radian) * len;
			real y = sin(radian) * len;
			radian -= PI*2 / bowl_part_count;

			if (x > -0.5f && x < 0.5f && y < 0)
				continue;
			bowl->particles.push_back(new Particle(vec2(x, y), vec2(0.0f, 0.0f), 0.5f, radius/1.5f, mass, 1));
		}
		radian = PI;
		for (int n = 0; n < bowl_part_count + 1; n++)
		{
			real x = cos(radian) * len;
			real y = sin(radian) * len;
			y -= len*2;
			radian -= PI*2 / bowl_part_count;

			if (x > -0.5f && x < 0.5f && y > -len*2)
				continue;
			bowl->particles.push_back(new Particle(vec2(x, y), vec2(0.0f, 0.0f), 0.5f, radius/1.5f, mass, 1));
		}

		// Create the sand
		// ---------------

		Cluster* sand = new Cluster;
		real width = 50;
		real height = 50;
		radius = 0.1f;
		bool shifted = 0;
		mass = PI * radius * radius	;
		Particle* quadp[15][8];
		for (real x = width / -2; x < width / 2; x++)
			for (real y = -height/2+5; y < height/2+5; y++)
			{
				vec2 pos((x * radius * 2), (y * radius * 1.73f) - 1);
				if (shifted)
					pos.x += radius;
				shifted = !shifted;
				if (length(pos) < len - 0.23f)
					sand->particles.push_back(new Particle(pos, vec2(0.0f, 0.0f), 0.5f, radius, mass, 0));
			}


		// Add clusters to world
		world->clusters.push_back(sand);
		world->clusters.push_back(bowl);
	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_DARK_PURPLE;
		colours.spring = Palette::JP_DARK_GRAY;
		colours.particle = Palette::JP_PURPLE;
		colours.particle_outline = Palette::JP_DARK_GRAY;
		colours.static_particle = Palette::JP_DARK_AQUA;
		colours.static_particle_outline = Palette::JP_DARK_GRAY;
	}

	void SetUpSceneSettings() override
	{
		settings.Reset();

		settings.attraction_tool_strength = 25.0f;
		settings.repulsion_tool_strength = 25.0f;

		settings.gravity.Set(0.0f, -9.8f);
		settings.circle_res = 10;
		settings.chunk_scale = 0.6f;
		settings.sub_steps = 25;

		camera.center.Set(0.0f, -3.25f);
		camera.zoom = 1.5f;
	}
};

#endif
