#ifndef SQUISHY_SQUARE_SCENE
#define SQUISHY_SQUARE_SCENE

#include "../Scene.hpp"
#include "../Settings.hpp"
#include "JoePhys/Particle.hpp"
#include "JoePhys/Spring.hpp"

struct SquishySquareScene : public Scene
{
	// constructors & destructors
	SquishySquareScene()
	{
		SetUpSceneColours();
		world = new World(settings.simulation_hertz, settings.sub_steps, vec2(0.0f, -980.0f));
		settings.circle_res = 20; //20
		double PI = 3.141592653589;
		
		// Create a floor
		// --------------

		Cluster* floor = new Cluster;
		for (int x = -12; x < 12; x++)
		{
			real radius = 10.0f;
			vec2 pos(x * radius * 2.0f, -250);
			vec2 vel(0.0f, 0.0f);
			real elasticity = 0.9f;
			real mass = 0.0f;
			Particle* p = new Particle(pos, vel, elasticity, radius, mass);

			// Add particle to floor cluster
			floor->particles.push_back(p);
		}

		// Create a square
		// ---------------
	
		Cluster* square = new Cluster;

		// Top Left Point
		real radius = 20.0f;
		vec2 pos(100.0f, -100.0f);
		/* vec2 vel(50.0f, 900.0f); */
		vec2 vel(0.0f, 0.0f);
		real elasticity = 0.9f;
		real mass = (real)PI * radius * radius;
		Particle* topLeft = new Particle(pos, vel, elasticity, radius, mass);
		square->particles.push_back(topLeft);
		
		// Top Right Point
		pos.Set(100.0f, 100.0f);
		vel.Set(0.0f, 0.0f);
		Particle* topRight = new Particle(pos, vel, elasticity, radius, mass);
		square->particles.push_back(topRight);
		
		// Bottom Left Point
		pos.Set(-100.0f, -100.0f);
		Particle* bottomLeft = new Particle(pos, vel, elasticity, radius, mass);
		square->particles.push_back(bottomLeft);
		
		// Bottom Right Point
		pos.Set(-100.0f, 100.0f);
		Particle* bottomRight = new Particle(pos, vel, elasticity, radius, mass);
		square->particles.push_back(bottomRight);

		// Add Springs To Square
		// ---------------------

		real len = 200.0f;
		Spring* topSpring = new Spring(topLeft, topRight, len);
		Spring* bottomSpring = new Spring(bottomLeft, bottomRight, len);
		Spring* leftSpring = new Spring(topLeft, bottomLeft, len);
		Spring* rightSpring = new Spring(topRight, bottomRight, len);
		Spring* topLeftToBottomRight = new Spring(topLeft, bottomRight, sqrt(len*len*2));
		Spring* topRightToBottomLeft = new Spring(topRight, bottomLeft, sqrt(len*len*2));

		// Add springs to cluster
		square->springs.push_back(topSpring);
		square->springs.push_back(bottomSpring);
		square->springs.push_back(leftSpring);
		square->springs.push_back(rightSpring);
		square->springs.push_back(topLeftToBottomRight);
		square->springs.push_back(topRightToBottomLeft);

		// Add clusters to world
		world->clusters.push_back(floor);
		world->clusters.push_back(square);
	}

	void SetUpSceneColours() override
	{
		settings.scene_colours.background = Palette::JP_BLUE;
		settings.scene_colours.spring = Palette::JP_GREEN;
		settings.scene_colours.particle = Palette::JP_DARK_PURPLE;
		settings.scene_colours.particle_outline = Palette::JP_DARK_GRAY;
		settings.scene_colours.static_particle = Palette::JP_YELLOW;
		settings.scene_colours.static_particle_outline = Palette::JP_DARK_YELLOW;
	}
};

#endif
