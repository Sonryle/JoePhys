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
		settings.circle_res = 20;

		// Create a physics object to go in our world
		PhysObj* myPhysObj = new PhysObj;

		// Create a 10x10 grid of particles to go in our physics object
		for (int y = 0; y < 10; y++)
			for (int x = 0; x < 10; x++)
			{
				vec2 pos(20.0f * (x + 0), 20.0f * (y - 12));
				vec2 vel(0.0f, 0.0f);
				real elasticity = 0.9f;
				real radius = 10;
				real mass = 1.0f;
				Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass);
				// Add the particle to the physics object
				myPhysObj->particles.push_back(myParticle);
			}

		// Add a ball to bounce into the other particles
		vec2 pos(-100.0f, 0.0f);
		vec2 vel(130.0f, 600.0f);
		real elasticity = 0.5f;
		real radius = 30;
		real mass = 4.0f;
		Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass);
		// Add the particle to the physics object
		myPhysObj->particles.push_back(myParticle);

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
