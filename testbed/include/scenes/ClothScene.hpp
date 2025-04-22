#ifndef CLOTH_SCENE
#define CLOTH_SCENE

// TEMPORARY FOR "stderr"
#include <cstdio>

#include "../Scenes.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"
#include "JoePhys/Particle.hpp"
#include "JoePhys/Spring.hpp"

struct ClothScene : public Scene
{
	// constructors & destructors
	void SetUpScene() override
	{
		SetUpSceneColours();
		world->Create(settings.simulation_hertz, settings.sub_steps, settings.gravity);
		double PI = 3.141592653589;
		
		// Create the cloth
		// ----------------
	
		Cluster* cloth = new Cluster;

		Particle* hp[49][28];
		for (int x = 0; x < 49; x++)
		{
			for (int y = 0; y < 28; y++)
			{
				// Add Particles
				real rad = 0.05f;
				vec2 pos(((x - 24) * 1.5f) * 0.15f, ((y - 14) * 1.5f) * 0.15f);
				vec2 vel(0.0f, 0.0f);
				real elas = 0.5f;
				real mass = (real)PI * rad * rad;
				bool is_static = 0;
				if (y == 27 && (x == 0 || x == 6 || x == 12 || x == 18 || x == 24 || x == 30 || x == 36 || x == 42 || x == 48))
					is_static = 1;
				hp[x][y] = new Particle(pos, vel, elas, rad, mass, is_static);
				cloth->particles.push_back(hp[x][y]);

				// Add Springs
				real stiffness = 100.0f;
				if (x > 0)
				{
					real len = length(hp[x][y]->pos - hp[x-1][y]->pos);
					Spring* s = new Spring(hp[x][y], hp[x-1][y], len, stiffness);
					cloth->springs.push_back(s);
				}
				if (y > 0)
				{
					real len = length(hp[x][y]->pos - hp[x][y-1]->pos);
					Spring* s = new Spring(hp[x][y], hp[x][y-1], len, stiffness);
					cloth->springs.push_back(s);
				}
			}
		}

		// Add clusters to world
		world->clusters.push_back(cloth);
	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_DARK_GREEN;
		colours.spring = Palette::JP_DARK_GRAY;
		colours.particle = Palette::JP_DARK_GREEN;
		colours.particle_outline = Palette::JP_DARK_GRAY;
		colours.static_particle = Palette::JP_GRAY;
		colours.static_particle_outline = Palette::JP_DARK_GRAY;
	}

	void SetUpSceneSettings() override
	{
		settings.Reset();
		settings.gravity.Set(0.0f, -9.8f);
		settings.enable_collision = 0;
		settings.attraction_tool_radius = 2.0f;
		settings.attraction_tool_strength = 1.0f;
		settings.repulsion_tool_radius = 2.0f;
		settings.repulsion_tool_strength = 8.0f;
		settings.min_particle_size = 0.05f;
		settings.max_particle_size = 1.0f;
		settings.circle_res = 5;
		settings.sub_steps = 4;
		camera.center.Set(0.0f, 0.0f);
		camera.zoom = 1.0f;
	}
};

#endif
