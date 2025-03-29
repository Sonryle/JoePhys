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
	ClothScene()
	{
		SetUpSceneColours();
		settings.Reset();
		world->Create(settings.simulation_hertz, settings.sub_steps, vec2(0.0f, -9.8f));
		settings.enable_collision = 0;
		settings.attraction_tool_strength = 2.0f;
		settings.repulsion_tool_strength = 4.0f;
		settings.circle_res = 5;
		camera.zoom = 1.0f;
		double PI = 3.141592653589;
		
		// Create the cloth
		// ----------------
	
		Cluster* cloth = new Cluster;

		Particle* hp[25][14];
		for (int x = 0; x < 25; x++)
		{
			for (int y = 0; y < 14; y++)
			{
				// Add Particles
				real rad = 0.05f;
				vec2 pos(((x - 12) * 3) * 0.15f, ((y - 6) * 3) * 0.15f);
				vec2 vel(0.0f, 0.0f);
				real elas = 0.5f;
				real mass = (real)PI * rad * rad;
				bool is_static = 0;
				if (y == 13 && (x == 0 || x == 6 || x == 12 || x == 18 || x == 24))
					is_static = 1;
				hp[x][y] = new Particle(pos, vel, elas, rad, mass, is_static);
				cloth->particles.push_back(hp[x][y]);

				// Add Springs
				real stiffness = 25.0f;
				if (x > 0)
				{
					real len = length(hp[x][y]->pos_in_meters - hp[x-1][y]->pos_in_meters);
					Spring* s = new Spring(hp[x][y], hp[x-1][y], len, stiffness);
					cloth->springs.push_back(s);
				}
				if (y > 0)
				{
					real len = length(hp[x][y]->pos_in_meters - hp[x][y-1]->pos_in_meters);
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
		colours.background = Palette::JP_RED;
		colours.spring = Palette::JP_DARK_YELLOW;
		colours.particle = Palette::JP_YELLOW;
		colours.particle_outline = Palette::JP_DARK_GRAY;
		colours.static_particle = Palette::JP_GRAY;
		colours.static_particle_outline = Palette::JP_DARK_GRAY;
	}
};

#endif
