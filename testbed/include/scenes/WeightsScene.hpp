#ifndef WEIGHTS_SCENE
#define WEIGHTS_SCENE

#include "../Scenes.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"
#include "JoePhys/Particle.hpp"

#include <cstdio>	// for "stderr" file path constant

struct WeightsScene : public Scene
{
	// constructors & destructors
	void SetUpScene() override
	{
		SetUpSceneColours();
		world->Create(settings.simulation_hertz, settings.sub_steps, settings.gravity);
		double PI = 3.141592653589;

		// Create a cluster for the particles to live in
		Cluster* obstacles = new Cluster;

		// Add two particles to the cluster
		real ob_rad = 0.544f;
		obstacles->particles.push_back(new Particle(vec2(0, 2.3), vec2(0, 0), 0.2f, ob_rad, PI * ob_rad*ob_rad, 1));

		// Create a rope
		// -------------
		Cluster* rope = new Cluster;
		real spring_stiffness = 10000;
		vec2 current_pos(-0.632f, -2);
		real dir = 0;

		// Create left boulder
		real lrad = 0.5f;
		Particle* l_boulder = new Particle(current_pos, vec2(0, 0), 0.5f, lrad, PI * lrad * lrad, 0);
		rope->particles.push_back(l_boulder);
		current_pos += vec2(sin(dir), cos(dir)) * (lrad - 0.1f);
		Particle* old_p = l_boulder;

		// Create left handing rope
		for (int n = 0; n < 20; n++)
		{
			real rad = 0.1f;
			current_pos += vec2(sin(dir), cos(dir)) * rad * 2;
			Particle* p = new Particle(current_pos, vec2(0, 0), 0.5f, rad, PI * rad * rad, 0);
			rope->particles.push_back(p);
			rope->springs.push_back(new Spring(p, old_p, length(p->pos - old_p->pos), spring_stiffness));
			old_p = p;
		}

		// Create curve
		real curve_segments = 10;
		for (int n = 0; n < curve_segments; n++)
		{
			real rad = 0.1f;
			dir += (PI / 2) / (curve_segments/2);
			current_pos += vec2(sin(dir), cos(dir)) * rad * 2;
			Particle* p = new Particle(current_pos, vec2(0, 0), 0.5f, rad, PI * rad * rad, 0);
			rope->particles.push_back(p);
			rope->springs.push_back(new Spring(p, old_p, length(p->pos - old_p->pos), spring_stiffness));
			old_p = p;
		}

		// Create right handing rope
		for (int n = 0; n < 18; n++)
		{
			real rad = 0.1f;
			current_pos += vec2(sin(dir), cos(dir)) * rad * 2;
			Particle* p = new Particle(current_pos, vec2(0, 0), 0.5f, rad, PI * rad * rad, 0);
			rope->particles.push_back(p);
			rope->springs.push_back(new Spring(p, old_p, length(p->pos - old_p->pos), spring_stiffness));
			old_p = p;
		}

		// Create right boulder
		real rrad = 0.4f;
		current_pos += vec2(sin(dir), cos(dir)) * (rrad + 0.1f);
		Particle* r_boulder = new Particle(current_pos, vec2(0, 0), 0.5f, rrad, PI * rrad * rrad, 0);
		rope->particles.push_back(r_boulder);
		rope->springs.push_back(new Spring(r_boulder, old_p, length(r_boulder->pos - old_p->pos), spring_stiffness));

		// Add cluster to the world
		world->clusters.push_back(obstacles);
		world->clusters.push_back(rope);
	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_DARK_WHITE;
		colours.spring = Palette::JP_DARK_GRAY;
		colours.particle = Palette::JP_GREEN;
		colours.particle_outline = Palette::JP_DARK_GRAY;
		colours.static_particle = Palette::JP_DARK_GREEN;
		colours.static_particle_outline = Palette::JP_DARK_GRAY;
	}

	void SetUpSceneSettings() override
	{
		settings.Reset();

		settings.gravity.Set(0.0f, -9.8f);
		settings.circle_res = 25;
		settings.chunk_scale = 1.0f;
		settings.sub_steps = 40;

		camera.center.Set(0.0f, 0.0f);
		camera.zoom = 1.0f;
	}
};

#endif
