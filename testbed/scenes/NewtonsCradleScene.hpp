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
		settings.Reset();
		world->Create(settings.simulation_hertz, settings.sub_steps, vec2(0.0f, -9.8f));
		settings.circle_res = 20;
		camera.zoom = 2.0f;
		double PI = 3.141592653589;


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

		// Add a ball to hit the 4 balls
		Cluster* cradle = new Cluster;

		for (int x = 0; x < 5; x++)
		{
			vec2 vel(0.0f, 0.0f);
			real elas = 1.0f;
			real rad = 0.75f;
			vec2 pos(anchors->particles[x]->pos_in_meters.x, -2.0f);
			real mass = PI * rad * rad;
			if (x == 0)
			{
				real len = 6.0f;
				real radian = PI;
				radian = -3;
				real posx = cos(radian) * len;
				real posy = sin(radian) * len;
				posx += anchors->particles[x]->pos_in_meters.x;
				posy += 4.0f;

				pos.Set(posx, posy);
			}
			if (x == 1)
			{
				real len = 6.0f;
				real radian = (7.0f*PI) / 6.0f;
				radian = -2.5f;
				real xpos = cos(radian) * len;
				real ypos = sin(radian) * len;
				xpos += anchors->particles[x]->pos_in_meters.x;
				ypos += 4.0f;

				pos.Set(xpos, ypos);
			}
			Particle* p = new Particle(pos, vel, elas, rad, mass);
			
			cradle->particles.push_back(p);
		}



		// Add springs connecting the balls to their anchor points
		for (int n = 0; n < 5; n++)
		{
			Particle* pA = cradle->particles[n];
			Particle* pB = anchors->particles[n];
			real len = 6.0f;
			real stiffness = 20000.0f;
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
