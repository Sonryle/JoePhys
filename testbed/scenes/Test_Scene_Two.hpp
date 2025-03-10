#ifndef JP_TEST_SCENE_TWO
#define JP_TEST_SCENE_TWO

#include "../Scene.hpp"
#include "../Settings.hpp"
#include "JoePhys/PhysObj.hpp"

struct TestSceneTwo : public Scene
{
	// constructors & destructors
	TestSceneTwo()
	{
		SetUpSceneColours();
		world = new World(settings.simulation_hertz, vec2(0, -980));
		settings.circle_res = 9;

		// Create a physics object to go in our world
		PhysObj* myPhysObj = new PhysObj;

		// Create a 5x5 grid of particles to go in our physics object
		for (int y = -2; y < 3; y++)
			for (int x = -2; x < 3; x++)
			{
				vec2 pos(60 * x, 60 * y);
				vec2 vel(x * 100, x * 100);
				real elasticity = 0.9f;
				real radius = 30;
				real mass = 1.0f;
				Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass);
				// Add the particle to the physics object
				myPhysObj->particles.push_back(myParticle);
			}

		// Add the physics object to the world
		world->PhysObjects.push_back(myPhysObj);
	}

	void SetUpSceneColours() override
	{
		settings.scene_colours.background = Palette::JP_GRAY;
		settings.scene_colours.circle_outlines = Palette::JP_DARK_GRAY;
		settings.scene_colours.circles = Palette::JP_RED;
	}
};

#endif
