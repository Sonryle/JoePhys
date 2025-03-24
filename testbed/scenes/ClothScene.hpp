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
		world->Create(settings.simulation_hertz, settings.sub_steps, vec2(0.0f, -9.8f));
		settings.circle_res = 20;
		camera.zoom = 1.0f;
		double PI = 3.141592653589;
		
		// Create static points to hold cloth up
		// -------------------------------------

		Cluster* pins = new Cluster;
		for (int x = -0; x < 2; x++)
		{
			real radius = 0.075f;
			real y = 6;
			vec2 pos((x-1) * 12 + 6, ((y-2) * 3) * 0.3f);
			vec2 vel(0.0f, 0.0f);
			real elasticity = 0.5f;
			real mass = 0.0f;
			Particle* p = new Particle(pos, vel, elasticity, radius, mass);

			// Add particle to floor cluster
			pins->particles.push_back(p);
		}

		// Create the cloth
		// ----------------
	
		Cluster* cloth = new Cluster;

		// Create a 7x14 grid of particles, connected by springs
		Particle* hp[24][14];
		for (int x = 0; x < 24; x++)
		{
			for (int y = 0; y < 14; y++)
			{
				// Add Particles
				real rad = 0.05f;
				vec2 pos(((x - 12) * 3) * 0.15f, ((y - 6) * 3) * 0.15f);
				vec2 vel(0.0f, 0.0f);
				real elas = 0.5f;
				real mass = (real)PI * rad * rad;
				hp[x][y] = new Particle(pos, vel, elas, rad, mass);
				cloth->particles.push_back(hp[x][y]);

				// Add Springs
				real stiffness = 15.0f;
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
				if (y > 0 && x > 0)
				{
					real len = length(hp[x-1][y]->pos_in_meters - hp[x][y-1]->pos_in_meters);
					real len2 = length(hp[x][y]->pos_in_meters - hp[x-1][y-1]->pos_in_meters);
					Spring* s = new Spring(hp[x-1][y], hp[x][y-1], len, stiffness / 2);
					Spring* s2 = new Spring(hp[x][y], hp[x-1][y-1], len2, stiffness / 2);
					cloth->springs.push_back(s);
					cloth->springs.push_back(s2);
				}
			}
		}
		real stiffness = 1000.0f;
		real leng = length(hp[0][13]->pos_in_meters - pins->particles[0]->pos_in_meters);
		Spring* s = new Spring(hp[0][13], pins->particles[0], leng, stiffness);
		real leng2 = length(hp[23][13]->pos_in_meters - pins->particles[0]->pos_in_meters);
		Spring* s2 = new Spring(hp[23][13], pins->particles[1], leng, stiffness);
		cloth->springs.push_back(s);
		cloth->springs.push_back(s2);

		// Add clusters to world
		world->clusters.push_back(pins);
		world->clusters.push_back(cloth);
	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_RED;
		colours.spring = Palette::JP_YELLOW;
		colours.particle = Palette::JP_DARK_YELLOW;
		colours.particle_outline = Palette::JP_DARK_GRAY;
		colours.static_particle = Palette::JP_GRAY;
		colours.static_particle_outline = Palette::JP_DARK_GRAY;
	}
};

#endif
