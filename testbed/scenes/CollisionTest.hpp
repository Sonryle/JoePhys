#ifndef JP_COLLISION_TEST_SCENE
#define JP_COLLISION_TEST_SCENE

#include "../Scene.hpp"
#include "../Settings.hpp"
#include "JoePhys/PhysObj.hpp"

struct CollisionTestScene : public Scene
{
	// constructors & destructors
	CollisionTestScene()
	{
		SetUpSceneColours();
		world = new World(settings.simulation_hertz, vec2(0.0f, 0.0f));
		settings.circle_res = 15;

		// Create a physics object to go in our world
		PhysObj* myPhysObj = new PhysObj;

		// Add two particles to the physics object
		vec2 posA(-100, -100);
		vec2 velA(100.0f, 100.0f);
		real radiusA = 50;
		real massA = M_PI * radiusA * radiusA;
		Particle* partA = new Particle(posA, velA, radiusA, massA);

		vec2 posB(100, 100);
		vec2 velB(-10.0f, -20.0f);
		real radiusB = 100;
		real massB = M_PI * radiusA * radiusA;
		Particle* partB = new Particle(posB, velB, radiusB, massB);
		myPhysObj->particles.push_back(partA);
		myPhysObj->particles.push_back(partB);

		// Add the physics object to the world
		world->PhysObjects.push_back(myPhysObj);
	}

	void SetUpSceneColours() override
	{
		settings.scene_colours.background = Palette::JP_AQUA;
		settings.scene_colours.circle_outlines = Palette::JP_DARK_GRAY;
		settings.scene_colours.circles = Palette::JP_GREEN;
	}
};

#endif
