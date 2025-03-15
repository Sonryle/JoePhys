#ifndef JP_TEST_SCENE
#define JP_TEST_SCENE

#include "../Scene.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"
#include "JoePhys/Particle.hpp"

struct TestScene : public Scene
{
	// constructors & destructors
	TestScene()
	{
		SetUpSceneColours();
		world = new World(settings.simulation_hertz, settings.sub_steps, vec2(0.0f, -980));
		settings.circle_res = 20; //20
		camera.center.Set( 0.0f, 350.0f);
		camera.zoom = 1.4f;
		double PI = 3.141592653589;

		// Create a cluster for the grid of particles (sand) to go in
		Cluster* sand = new Cluster;

		// Create an 11x11 grid of particles to go in our cluster
		for (int y = -5; y < 6; y++)
			for (int x = -4; x < 5; x++)
			{
				vec2 vel(0.0f, 0.0f);
				vec2 pos((50.0f * x) + 5, (40.0f * y) + 800);
				real elasticity = 0.9f;
				real radius = (x + 7) * 1.5f;
				real mass = (float)PI * radius * radius;
				Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass);
				// Add the particle to the world
				sand->particles.push_back(myParticle);
			}

		// Create a cluster for the ground
		Cluster* floor = new Cluster;
		// Add a line of particles as the ground
		for (int x = -12; x < 12; x++)
		{
			vec2 vel(0.0f, 0.0f);
			real radius = 10;
			vec2 pos(radius * 2.0f * x, -250);
			real elasticity = 0.9f;
			real mass = 0.0f;
			Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass);
			// Add the particle to the physics object
			floor->particles.push_back(myParticle);
		}
		// Add a line of particles as the left wall
		for (int y = -10; y < 30; y++)
		{
			vec2 vel(0.0f, 0.0f);
			real radius = 10;
			vec2 pos(-240, (y * 2.0f * radius) - 50);
			real elasticity = 0.9f;
			real mass = 0.0f;
			Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass);
			// Add the particle to the physics object
			floor->particles.push_back(myParticle);
		}
		// Add a line of particles as the right wall
		for (int y = -10; y < 30; y++)
		{
			vec2 vel(0.0f, 0.0f);
			real radius = 10;
			vec2 pos(240, (y * 2.0f * radius) - 50);
			real elasticity = 0.9f;
			real mass = 0.0f;
			Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass);
			// Add the particle to the physics object
			floor->particles.push_back(myParticle);
		}
		// Add particles inside of box for sand particles to bounce off of
		for (int y = 0; y < 3; y++)
		{
			for (int x = -2; x < 3; x++)
			{
				vec2 vel(0.0f, 0.0f);
				real radius = 15;
				vec2 pos(x * 90, y * 200);
				real elasticity = 0.9f;
				real mass = 0.0f;
				Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass);
				// Add the particle to the physics object
				floor->particles.push_back(myParticle);
			}
			for (int x = -2; x < 2; x++)
			{
				vec2 vel(0.0f, 0.0f);
				real radius = 20;
				vec2 pos((x * 90) + 45, (y * 200) - 100);
				real elasticity = 0.9f;
				real mass = 0.0f;
				Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass);
				// Add the particle to the physics object
				floor->particles.push_back(myParticle);
			}
		}

		// Create a basket to catch the odd flying ball
		for (int x = 0; x < 5; x++)
		{
			vec2 vel(0.0f, 0.0f);
			real radius = 10;
			vec2 pos((radius * 2.0f * x) - 680, 450);
			real elasticity = 0.9f;
			real mass = 0.0f;
			Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass);
			// Add the particle to the physics object
			floor->particles.push_back(myParticle);
			
			vel.Set(0.0f, 0.0f);
			radius = 10;
			pos.Set(-600, (radius * 2.0f * x) + 450);
			elasticity = 0.9f;
			mass = 0.0f;
			myParticle = new Particle(pos, vel, elasticity, radius, mass);
			// Add the particle to the physics object
			floor->particles.push_back(myParticle);

			vel.Set(0.0f, 0.0f);
			radius = 10;
			pos.Set(-680, (radius * 2.0f * x) + 450);
			elasticity = 0.9f;
			mass = 0.0f;
			myParticle = new Particle(pos, vel, elasticity, radius, mass);
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
		settings.scene_colours.particle = Palette::JP_AQUA;
		settings.scene_colours.particle_outline = Palette::JP_DARK_GRAY;
		settings.scene_colours.static_particle = Palette::JP_DARK_AQUA;
		settings.scene_colours.static_particle_outline = Palette::JP_DARK_GRAY;
	}
};

#endif
