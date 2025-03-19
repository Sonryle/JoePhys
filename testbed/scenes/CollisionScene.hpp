#ifndef JP_COLLISION_SCENE
#define JP_COLLISION_SCENE

#include "../Scene.hpp"
#include "../Settings.hpp"
#include "JoePhys/Particle.hpp"

struct CollisionScene : public Scene
{
	// constructors & destructors
	CollisionScene()
	{
		SetUpSceneColours();
		world = new World(settings.simulation_hertz, settings.sub_steps, vec2(0.0f, 0.0f));
		settings.circle_res = 25;
		double PI = 3.141592653589;

		// Create a cluster for the particles to live in
		Cluster* c = new Cluster;

		// Add two particles to the cluster
		vec2 posA(-1, -1);
		vec2 velA(1.0f, 2.0f);
		real elasticityA = 0.9f;
		real radiusA = 0.5f;
		real massA = (float)PI * radiusA * radiusA;
		Particle* partA = new Particle(posA, velA, elasticityA, radiusA, massA);
		c->particles.push_back(partA);

		vec2 posB(1, 1);
		vec2 velB(-0.1f, -0.2f);
		real elasticityB = 0.9f;
		real radiusB = 1;
		real massB = (float)PI * radiusB * radiusB;
		Particle* partB = new Particle(posB, velB, elasticityB, radiusB, massB);
		c->particles.push_back(partB);

		// Add cluster to the world
		world->clusters.push_back(c);
	}

	void SetUpSceneColours() override
	{
		settings.scene_colours.background = Palette::JP_AQUA;
		settings.scene_colours.spring = Palette::JP_GREEN;
		settings.scene_colours.particle = Palette::JP_GREEN;
		settings.scene_colours.particle_outline = Palette::JP_DARK_GRAY;
		settings.scene_colours.static_particle = Palette::JP_DARK_GREEN;
		settings.scene_colours.static_particle_outline = Palette::JP_DARK_GRAY;
	}
};

#endif
