#ifndef AIR_RESISTANCE_SCENE
#define AIR_RESISTANCE_SCENE

// TEMPORARY FOR "stderr"
#include <cstdio>

#include "../Scenes.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"
#include "JoePhys/Particle.hpp"
#include "JoePhys/Spring.hpp"

struct AirResistanceScene : public Scene
{
	// constructors & destructors
	void SetUpScene() override
	{
		SetUpSceneColours();
		world->Create(settings.simulation_hertz, settings.sub_steps, settings.gravity);
		double PI = 3.141592653589;

		Cluster* balls = new Cluster;
	
		for (int y = -5; y < 6; y++)
			for (int x = -4; x < 5; x++)
			{
				vec2 vel(0.0f, 0.0f);
				vec2 pos((0.5f * x) + 0.05f, (0.4f * y) + 8.0f);
				real radius = (x + 7) * 0.015f;
				real mass = (float)PI * radius * radius;
				real elasticity = 0.8f;
				Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass, 0);
				// Add the particle to the world
				balls->particles.push_back(myParticle);
			}
		
		// Add clusters to world
		world->clusters.push_back(balls);
	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_DARK_GRAY;
		colours.particle = Palette::JP_PURPLE;
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

		camera.center.Set(0.0f, 2.0f);
		camera.zoom = 1.0f;
	}
};

#endif
