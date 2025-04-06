#ifndef JP_ROPE_SCENE
#define JP_ROPE_SCENE

#include "../Scenes.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"
#include "JoePhys/Particle.hpp"

#include <cstdio>	// for "stderr" file path constant

struct RopeScene : public Scene
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
		vec2 posA(-2.66f, -0);
		vec2 velA(0.0f, 0.0f);
		real elasticityA = 0.2f;
		real radiusA = 1.5f;
		real massA = PI * radiusA * radiusA;
		Particle* partA = new Particle(posA, velA, elasticityA, radiusA, massA, 1);
		obstacles->particles.push_back(partA);

		vec2 posB(3, 0);
		vec2 velB(-0.0f, -0.0f);
		real elasticityB = 0.2f;
		real radiusB = 1;
		real massB = PI * radiusB * radiusB;
		Particle* partB = new Particle(posB, velB, elasticityB, radiusB, massB, 1);
		obstacles->particles.push_back(partB);

		// Create a rope
		Cluster* rope = new Cluster;
		Particle* old_part = nullptr;
		for (int x = -28; x < 28; x++)
		{
			vec2 vel(0.0f, 0.0f);
			real radius = 0.1f;
			vec2 pos(radius * 3.0f * x, 2.5f);
			real elasticity = 0.2f;
			real mass = PI * radius * radius;
			Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass, 0);
			// Add the particle to the physics object
			rope->particles.push_back(myParticle);

			real stiffness = 2500.0f;
			if (old_part != nullptr)
			{
				real len = length(old_part->pos_in_meters - myParticle->pos_in_meters);
				Spring* s = new Spring(old_part, myParticle, len, stiffness , len + 2, -1);
				rope->springs.push_back(s);
			}
			old_part = myParticle;
		}

		// Add cluster to the world
		world->clusters.push_back(obstacles);
		world->clusters.push_back(rope);
	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_PURPLE;
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

		camera.center.Set(0.0f, 0.0f);
		camera.zoom = 1.0f;
	}
};

#endif
