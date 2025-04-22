#ifndef WRECKING_BALL_SCENE
#define WRECKING_BALL_SCENE

// TEMPORARY FOR "stderr"
#include <cstdio>

#include "../Scenes.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"
#include "JoePhys/Particle.hpp"
#include "JoePhys/Spring.hpp"

struct WreckingBallScene : public Scene
{
	// constructors & destructors
	void SetUpScene() override
	{
		SetUpSceneColours();
		world->Create(settings.simulation_hertz, settings.sub_steps, settings.gravity);
		double PI = 3.141592653589;

		// Create a wrecking ball
		Cluster* wrecking_ball = new Cluster;
		wrecking_ball->particles.push_back(new Particle(vec2(-2, 3), vec2(0, 0), 0.5f, 0.1f, 0, 1));
		wrecking_ball->particles.push_back(new Particle(vec2(-7, 3), vec2(0, 0), 0.5f, 0.75f, 30, 0));
		wrecking_ball->springs.push_back(new Spring(wrecking_ball->particles[0], wrecking_ball->particles[1], 5, 5000));

		// Create a platform for particles
		Cluster* platform = new Cluster;
		for (int n = 0; n < 20; n++)
			platform->particles.push_back(new Particle(vec2(n * 0.2f, -3), vec2(0, 0), 0.5f, 0.1f, 0, 1));

		// Create the particles to be hit
		Cluster* debris = new Cluster;
		for (int x = 0; x < 20; x++)
			for (int y = 0; y < 20; y++)
				debris->particles.push_back(new Particle(vec2(x * 0.2f, y * 0.2f - 2.8f), vec2(0, 0), 0.5f, 0.1f, PI * 0.01f, 0));
		
		// Add clusters to world
		// ---------------------
		world->clusters.push_back(wrecking_ball);
		world->clusters.push_back(platform);
		world->clusters.push_back(debris);
	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_DARK_AQUA;
		colours.spring = Palette::JP_DARK_GRAY;
		colours.particle = Palette::JP_GREEN;
		colours.particle_outline = Palette::JP_DARK_GRAY;
		colours.static_particle = Palette::JP_GRAY;
		colours.static_particle_outline = Palette::JP_DARK_GRAY;
	}

	void SetUpSceneSettings() override
	{
		settings.Reset();

		settings.attraction_tool_strength = 25.0f;
		settings.repulsion_tool_strength = 25.0f;

		settings.gravity.Set(0.0f, -9.8f);
		settings.circle_res = 15;
		settings.chunk_scale = 0.75f;
		settings.sub_steps = 16;

		camera.center.Set(0.0f, 0.0f);
		camera.zoom = 1.0f;
	}
};

#endif
