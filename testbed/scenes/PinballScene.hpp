#ifndef JP_PINBALL_SCENE
#define JP_PINBALL_SCENE

#include "../Scenes.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"
#include "JoePhys/Particle.hpp"

struct PinballScene : public Scene
{
	// constructors & destructors
	PinballScene()
	{
		SetUpSceneColours();
		world = new World(settings.simulation_hertz, settings.sub_steps, vec2(0.0f, -9.8f));
		settings.circle_res = 20; //20
		camera.center.Set( 0.0f, 3.5f);
		camera.zoom = 1.4f;
		double PI = 3.141592653589;

		// Create a cluster for the grid of particles (sand) to go in
		Cluster* sand = new Cluster;

		// Create an 11x11 grid of particles to go in our cluster
		for (int y = -5; y < 6; y++)
			for (int x = -4; x < 5; x++)
			{
				vec2 vel(0.0f, 0.0f);
				vec2 pos((0.5f * x) + 0.05f, (0.4f * y) + 8.0f);
				real radius = (x + 7) * 0.015f;
				real mass = (float)PI * radius * radius;
				real elasticity = 0.8f;
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
			real radius = 0.1f;
			vec2 pos(radius * 2.0f * x, -2.5f);
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
			real radius = 0.1f;
			vec2 pos(-2.4f, (y * 2.0f * radius) - 0.5f);
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
			real radius = 0.1f;
			vec2 pos(2.4f, (y * 2.0f * radius) - 0.5f);
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
				real radius = 0.15f;
				vec2 pos(x * 0.9f, y * 2.0f);
				real elasticity = 0.9f;
				real mass = 0.0f;
				Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass);
				// Add the particle to the physics object
				floor->particles.push_back(myParticle);
			}
			for (int x = -2; x < 2; x++)
			{
				vec2 vel(0.0f, 0.0f);
				real radius = 0.2f;
				vec2 pos((x * 0.9f) + 0.45f, (y * 2.0f) - 1.0f);
				real elasticity = 0.9f;
				real mass = 0.0f;
				Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass);
				// Add the particle to the physics object
				floor->particles.push_back(myParticle);
			}
		}

		// Add cluster to world
		world->clusters.push_back(sand);
		world->clusters.push_back(floor);
	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_YELLOW;
		colours.spring = Palette::JP_GREEN;
		colours.particle = Palette::JP_AQUA;
		colours.particle_outline = Palette::JP_DARK_GRAY;
		colours.static_particle = Palette::JP_DARK_AQUA;
		colours.static_particle_outline = Palette::JP_DARK_GRAY;
	}
};

#endif
