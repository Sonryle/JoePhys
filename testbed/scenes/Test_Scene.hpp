#ifndef JP_TEST_SCENE
#define JP_TEST_SCENE

#include <stdlib.h>	// for 'srand' and 'rand'
#include <math.h>	// for M_PI
#include "../Scene.hpp"
#include "../Settings.hpp"
#include "JoePhys/PhysObj.hpp"

struct TestScene : public Scene
{
	// constructors & destructors
	TestScene()
	{
		SetUpSceneColours();
		world = new World(settings.simulation_hertz, vec2(0.0f, 0.0f));
		settings.circle_res = 12;

		// Create a physics object to go in our world
		PhysObj* myPhysObj = new PhysObj;

		// Create an 11x11 grid of particles to go in our physics object
		for (int y = -5; y < 6; y++)
			for (int x = -5; x < 6; x++)
			{
				// generate random velocity
				srand(x);
				real r = (real)(rand() % 10 + 1);
				real vel_x = sin(x + r);
				real vel_y = cos(y + r);
				vec2 vel(vel_x * 30, vel_y * 30);

				vec2 pos(40 * x, 40 * y);
				real radius = r * 3;
				real mass = M_PI * radius * radius;
				Particle* myParticle = new Particle(pos, vel, radius, mass);
				// Add the particle to the physics object
				myPhysObj->particles.push_back(myParticle);
			}

		// Add the physics object to the world
		world->PhysObjects.push_back(myPhysObj);
	}

	void SetUpSceneColours() override
	{
		settings.scene_colours.background = Palette::JP_YELLOW;
		settings.scene_colours.circle_outlines = Palette::JP_DARK_GRAY;
		settings.scene_colours.circles = Palette::JP_AQUA;
	}
};

#endif
