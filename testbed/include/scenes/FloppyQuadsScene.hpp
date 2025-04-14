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
		int bowl_part_count = 50;
		real radian = 2 * PI;
		real radius = 0.34f;
		real mass = PI * radius * radius;
		for (int n = 0; n < bowl_part_count + 1; n++)
		{
				real len = 12.0f;
				real x = cos(radian) * len;
				real y = sin(radian) * len;
				radian -= PI / bowl_part_count;

				Particle* p = new Particle(vec2(x, y), vec2(0.0f, 0.0f), 0.75f, radius, mass, 1);
				bowl->particles.push_back(p);
		}

		// Create a squishy quad
		// ---------------------

		Cluster* quad = new Cluster;
		real width = 15;
		real height = 8;
		real xoffset = -10;
		real yoffset = 0;
		radius = 0.15f;
		mass = PI * radius * radius;
		Particle* quadp[15][8];
		for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++)
			{
				Particle* p = new Particle(vec2((x * radius * 2) + xoffset, (y * radius * 2) + yoffset), vec2(0.0f, 0.0f), 0.75f, radius*0.9f, mass, 0);
				quad->particles.push_back(p);
				quadp[x][y] = p;
			}
		// Add springs
		for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++)
			{
				real stiffness = 100.0f;
				real len = radius * 2;
				if (x > 0)
					quad->springs.push_back(new Spring(quadp[x][y], quadp[x - 1][y], len, stiffness));
				if (y > 0)
					quad->springs.push_back(new Spring(quadp[x][y], quadp[x][y - 1], len, stiffness));
				if (x > 0 && y > 0)
					quad->springs.push_back(new Spring(quadp[x][y], quadp[x - 1][y - 1], sqrt(len * len + len * len), stiffness));
				if (x > 0 && y < height - 1)
					quad->springs.push_back(new Spring(quadp[x][y], quadp[x - 1][y + 1], sqrt(len * len + len * len), stiffness));
			}

		// Create another squishy quad
		// ---------------------------

		Cluster* quad2 = new Cluster;
		width = 4;
		height = 15;
		xoffset = 5;
		yoffset = 0;
		radius = 0.15f;
		mass = PI * radius * radius;
		Particle* quad2p[4][15];
		for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++)
			{
				Particle* p = new Particle(vec2((x * radius * 2) + xoffset, (y * radius * 2) + yoffset), vec2(0.0f, 0.0f), 0.75f, radius*0.9f, mass, 0);
				quad2->particles.push_back(p);
				quad2p[x][y] = p;
			}
		// Add springs
		for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++)
			{
				real stiffness = 10000.0f;
				real len = radius * 2;
				if (x > 0)
					quad2->springs.push_back(new Spring(quad2p[x][y], quad2p[x - 1][y], len, stiffness));
				if (y > 0)
					quad2->springs.push_back(new Spring(quad2p[x][y], quad2p[x][y - 1], len, stiffness));
				if (x > 0 && y > 0)
					quad2->springs.push_back(new Spring(quad2p[x][y], quad2p[x - 1][y - 1], sqrt(len * len + len * len), stiffness));
				if (x > 0 && y < height - 1)
					quad2->springs.push_back(new Spring(quad2p[x][y], quad2p[x - 1][y + 1], sqrt(len * len + len * len), stiffness));
			}
		

		// Add clusters to world
		world->clusters.push_back(quad);
		world->clusters.push_back(quad2);
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
		settings.sub_steps = 8;

		camera.center.Set(0.0f, 0.0f);
		camera.zoom = 2.0f;
	}
};

#endif
