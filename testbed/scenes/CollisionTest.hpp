#ifndef JP_COLLISION_TEST_SCENE
#define JP_COLLISION_TEST_SCENE

#include "../Scene.hpp"
#include "../Settings.hpp"
#include "JoePhys/Particle.hpp"

struct CollisionTestScene : public Scene
{
	// constructors & destructors
	CollisionTestScene()
	{
		SetUpSceneColours();
		world = new World(settings.simulation_hertz, settings.sub_steps, vec2(0.0f, -980.0f));
		settings.circle_res = 25;
		double PI = 3.141592653589;

		// Create a cluster for the particles to live in
		Cluster* c = new Cluster;

		// Add two particles to the cluster
		vec2 posA(-100, -100);
		vec2 velA(100.0f, 100.0f);
		real elasticityA = 1.0f;
		real radiusA = 50;
		real massA = (float)PI * radiusA * radiusA;
		Particle* partA = new Particle(posA, velA, elasticityA, radiusA, massA);
		c->particles.push_back(partA);

		vec2 posB(100, 100);
		vec2 velB(-10.0f, -20.0f);
		real elasticityB = 1.0f;
		real radiusB = 100;
		real massB = (float)PI * radiusA * radiusA;
		Particle* partB = new Particle(posB, velB, elasticityB, radiusB, massB);
		c->particles.push_back(partB);

		// Add cluster to the world
		world->clusters.push_back(c);
	}

	void SetUpSceneColours() override
	{
		settings.scene_colours.background = Palette::JP_AQUA;
		settings.scene_colours.circle_outlines = Palette::JP_DARK_GRAY;
		settings.scene_colours.circles = Palette::JP_GREEN;
	}
};

#endif
