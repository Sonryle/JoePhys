#ifndef JP_TEST_SCENE
#define JP_TEST_SCENE

#include "../Scene.hpp"
#include "../Settings.hpp"
#include "JoePhys/Particle.hpp"

struct TestScene : public Scene
{
	// constructors & destructors
	TestScene()
	{
		SetUpSceneColours();
		world = new World(settings.simulation_hertz, settings.sub_steps, vec2(0.0f, -980));
		settings.circle_res = 20; //20
		double PI = 3.141592653589;

		// Create a cluster for the grid of particles (sand) to go in
		Cluster* sand = new Cluster;

		// Create an 11x11 grid of particles to go in our cluster
		for (int y = -5; y < 6; y++)
			for (int x = 5; x < 15; x++)
			{
				vec2 vel(0.0f, 0.0f);
				vec2 pos(40.0f * (x - 10), 40.0f * y);
				real elasticity = 0.9f;
				real radius = x * 1.5f;
				real mass = (float)PI * radius * radius;
				Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass);
				// Add the particle to the world
				sand->particles.push_back(myParticle);
			}

		// Create a cluster for the ground
		Cluster* floor = new Cluster;
		// Add a line of particles as the ground
		for (int x = -20; x < 20; x++)
		{
			vec2 vel(0.0f, 0.0f);
			vec2 pos(20.0f * x, -160);
			real elasticity = 0.9f;
			real radius = 10;
			real mass = (float)PI * radius * radius;
			Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass);
			// Add the particle to the physics object
			floor->particles.push_back(myParticle);
		}

		// Add cluster to world
		world->clusters.push_back(sand);
		world->clusters.push_back(floor);
	}

	void SetUpSceneColours() override
	{
		settings.scene_colours.background = Palette::JP_YELLOW;
		settings.scene_colours.circle_outlines = Palette::JP_DARK_GRAY;
		settings.scene_colours.circles = Palette::JP_AQUA;
	}
};

#endif
