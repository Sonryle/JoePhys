#ifndef SQUISHY_RECTANGLE_SCENE
#define SQUISHY_RECTANGLE_SCENE

#include "../Scene.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"
#include "JoePhys/Particle.hpp"
#include "JoePhys/Spring.hpp"

struct SquishyRectangleScene : public Scene
{
	// constructors & destructors
	SquishyRectangleScene()
	{
		SetUpSceneColours();
		world = new World(settings.simulation_hertz, settings.sub_steps, vec2(0.0f, -980.0f));
		settings.circle_res = 20; //20
		camera.zoom = 1.0f;
		camera.center.Set(400.0f, -400.0f);
		double PI = 3.141592653589;
		
		// Create a floor
		// --------------

		Cluster* floor = new Cluster;
		for (int x = -20; x < 30; x++)
		{
			real radius = 2.5f;
			vec2 pos(x * radius * 2.0f, -250 - x * 2);
			vec2 vel(0.0f, 0.0f);
			real elasticity = 0.9f;
			real mass = 0.0f;
			Particle* p = new Particle(pos, vel, elasticity, radius, mass);

			// Add particle to floor cluster
			floor->particles.push_back(p);
		}
		for (int x = -20; x < 30; x++)
		{
			real radius = 2.5f;
			vec2 pos((x * radius * 2.0f) + 400.0f, -450.0f + x * 2);
			vec2 vel(0.0f, 0.0f);
			real elasticity = 0.9f;
			real mass = 0.0f;
			Particle* p = new Particle(pos, vel, elasticity, radius, mass);

			// Add particle to floor cluster
			floor->particles.push_back(p);
		}
		for (int x = -40; x < 40; x++)
		{
			real radius = 2.5f;
	 		vec2 pos((x * radius * 2.0f) + 50.0f, -650.0f + abs(x) * 1.5f);
			vec2 vel(0.0f, 0.0f);
			real elasticity = 0.9f;
			real mass = 0.0f;
			Particle* p = new Particle(pos, vel, elasticity, radius, mass);

			// Add particle to floor cluster
			floor->particles.push_back(p);
		}

		// Create a horizontal Rectangle
		// -----------------------------
	
		Cluster* horiz_rect = new Cluster;

		// Create a 7x14 grid of particles, connected by springs
		Particle* hp[7][14];
		for (int x = 0; x < 7; x++)
		{
			for (int y = 0; y < 14; y++)
			{
				// Add Particles
				vec2 pos((x * 20) - 3 * 20, (y * 20) - 200.0f);
				vec2 vel(0.0f, 0.0f);
				real elas = 0.9f;
				real rad = 5.0f;
				real mass = PI * rad * rad;
				hp[x][y] = new Particle(pos, vel, elas, rad, mass);
				horiz_rect->particles.push_back(hp[x][y]);

				// Add Springs
				if (x > 0)
				{
					Spring* s = new Spring(hp[x][y], hp[x-1][y], 20.0f);
					horiz_rect->springs.push_back(s);
				}
				if (y > 0)
				{
					Spring* s = new Spring(hp[x][y], hp[x][y-1], 20.0f);
					horiz_rect->springs.push_back(s);
				}
				if (y > 0 && x > 0)
				{
					Spring* s = new Spring(hp[x-1][y], hp[x][y-1], sqrt(800.0f));
					Spring* s2 = new Spring(hp[x][y], hp[x-1][y-1], sqrt(800.0f));
					horiz_rect->springs.push_back(s);
					horiz_rect->springs.push_back(s2);
				}
			}
		}

		// Add clusters to world
		world->clusters.push_back(floor);
		world->clusters.push_back(horiz_rect);
	}

	void SetUpSceneColours() override
	{
		settings.scene_colours.background = Palette::JP_DARK_GREEN;
		settings.scene_colours.spring = Palette::JP_YELLOW;
		settings.scene_colours.particle = Palette::JP_DARK_YELLOW;
		settings.scene_colours.particle_outline = Palette::JP_DARK_GRAY;
		settings.scene_colours.static_particle = Palette::JP_GRAY;
		settings.scene_colours.static_particle_outline = Palette::JP_DARK_GRAY;
	}
};

#endif
