#ifndef NEWTONS_CRADLE_SCENE
#define NEWTONS_CRADLE_SCENE

// TEMPORARY FOR "stderr"
#include <cstdio>

#include "../Scenes.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"
#include "JoePhys/Particle.hpp"
#include "JoePhys/Spring.hpp"

struct NewtonsCradleScene : public Scene
{
	// constructors & destructors
	NewtonsCradleScene()
	{
		SetUpSceneColours();
		world->Create(settings.simulation_hertz, settings.sub_steps, vec2(0.0f, -9.8f));
		settings.circle_res = 20;
		double PI = 3.141592653589;

		// Add a ball to hit the 4 balls
		Cluster* cradle = new Cluster;

		vec2 vel(0.0f, 0.0f);
		real elas = 1.0f;
		real rad = 0.75f;
		vec2 pos((-1 * rad * 2) - rad * 2 - 6.0f, 4.0f);
		real mass = PI * rad * rad;
		Particle* p = new Particle(pos, vel, elas, rad, mass);
		
		cradle->particles.push_back(p);

		// Create a cluster of 4 particles for the moving balls
		for (int x = 0; x < 4; x++)
		{
			vec2 vel(0.0f, 0.0f);
			real elas = 1.0f;
			real rad = 0.75f;
			vec2 pos((x * rad * 2) - rad * 2, -2.0f);
			if (x == 0)
				pos.Set((-1 * rad * 2) - rad * 2 - 6.0f, 2.5f);
			real mass = PI * rad * rad;
			Particle* p = new Particle(pos, vel, elas, rad, mass);
			
			cradle->particles.push_back(p);
		}

		// Create a cluster for the anchor points of the balls
		Cluster* anchors = new Cluster;
		for (int x = 0; x < 5; x++)
		{
			vec2 vel(0.0f, 0.0f);
			real elas = 1.0f;
			real rad = 0.25f;
			vec2 pos((x * 1.5f) - 3.0f, 4.0f);
			real mass = 0.0f;
			Particle* p = new Particle(pos, vel, elas, rad, mass);
			
			anchors->particles.push_back(p);
		}

		// Add springs connecting the balls to their anchor points
		for (int n = 0; n < 5; n++)
		{
			Particle* pA = cradle->particles[n];
			Particle* pB = anchors->particles[n];
			real len = 6.0f;
			real stiffness = 2000000.0f;
			Spring* s = new Spring(pA, pB, len, stiffness);

			anchors->springs.push_back(s);
		}
		
		// Add clusters to world
		world->clusters.push_back(cradle);
		world->clusters.push_back(anchors);
	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_DARK_YELLOW;
		colours.spring = Palette::JP_DARK_GRAY;
		/* colours.particle = Palette::JP_DARK_BLUE; */
		colours.particle = Palette::JP_GREEN;
		colours.particle_outline = Palette::JP_DARK_GRAY;
		colours.static_particle = Palette::JP_GRAY;
		colours.static_particle_outline = Palette::JP_DARK_GRAY;
	}
};

#endif
