#ifndef JP_PINBALL_SCENE
#define JP_PINBALL_SCENE

#include "../Scenes.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"
#include "JoePhys/Particle.hpp"

struct PinballScene : public Scene
{
	// constructors & destructors
	void SetUpScene() override
	{
		SetUpSceneColours();
		world->Create(settings.simulation_hertz, settings.sub_steps, settings.gravity);
		double PI = 3.141592653589;

		// Create a cluster for the grid of particles (sand) to go in
		Cluster* sand = new Cluster;

		// Create an 11x9 grid of particles to go in our cluster
		for (int y = -5; y < 6; y++)
			for (int x = -4; x < 5; x++)
			{
				vec2 vel(0.0f, 0.0f);
				vec2 pos((0.5f * x) + 0.05f, (0.4f * y) + 8.0f);
				real radius = (x + 7) * 0.015f;
				real mass = (float)PI * radius * radius;
				real elasticity = 0.8f;
				Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass, 0);
				// Add the particle to the world
				sand->particles.push_back(myParticle);
			}

		// Create a cluster for the ground
		Cluster* floor = new Cluster;
		// Add a line of particles as the ground
		for (int x = -12; x < 12; x++)
		{
			vec2 vel(0.0f, 0.0f);
			real radius = 0.1f;
			vec2 pos(radius * 2.0f * x, -2.5f);
			real elasticity = 0.9f;
			real mass = PI * radius * radius;
			Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass, 1);
			// Add the particle to the physics object
			floor->particles.push_back(myParticle);
		}
		// Add a line of particles as the left wall
		for (int y = -10; y < 30; y++)
		{
			vec2 vel(0.0f, 0.0f);
			real radius = 0.1f;
			vec2 pos(-2.4f, (y * 2.0f * radius) - 0.5f);
			real elasticity = 0.9f;
			real mass = PI * radius * radius;
			Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass, 1);
			// Add the particle to the physics object
			floor->particles.push_back(myParticle);
		}
		// Add a line of particles as the right wall
		for (int y = -10; y < 30; y++)
		{
			vec2 vel(0.0f, 0.0f);
			real radius = 0.1f;
			vec2 pos(2.4f, (y * 2.0f * radius) - 0.5f);
			real elasticity = 0.9f;
			real mass = PI * radius * radius;
			Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass, 1);
			// Add the particle to the physics object
			floor->particles.push_back(myParticle);
		}
		// Add particles inside of box for sand particles to bounce off of
		for (int y = 0; y < 3; y++)
		{
			for (int x = -2; x < 3; x++)
			{
				vec2 vel(0.0f, 0.0f);
				real radius = 0.15f;
				vec2 pos(x * 0.9f, y * 2.0f);
				real elasticity = 0.9f;
				real mass = PI * radius * radius;
				Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass, 1);
				// Add the particle to the physics object
				floor->particles.push_back(myParticle);
			}
			for (int x = -2; x < 2; x++)
			{
				vec2 vel(0.0f, 0.0f);
				real radius = 0.2f;
				vec2 pos((x * 0.9f) + 0.45f, (y * 2.0f) - 1.0f);
				real elasticity = 0.9f;
				real mass = PI * radius * radius;
				Particle* myParticle = new Particle(pos, vel, elasticity, radius, mass, 1);
				// Add the particle to the physics object
				floor->particles.push_back(myParticle);
			}
		}

		// Add cluster to world
		world->clusters.push_back(sand);
		world->clusters.push_back(floor);
	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_DARK_YELLOW;
		colours.particle = Palette::JP_YELLOW;
		colours.static_particle = Palette::JP_DARK_AQUA;
	}

	void SetUpSceneSettings() override
	{
		settings.Reset();

		settings.attraction_tool_radius = 2.0f;
		settings.attraction_tool_strength = 3.0f;
		settings.repulsion_tool_radius = 2.0f;
		settings.repulsion_tool_strength = 3.0f;

		settings.gravity.Set(0.0f, -9.8f);
		settings.circle_res = 20;

		settings.sub_steps = 32;
		settings.chunk_scale = 0.73f;

		camera.center.Set( 0.0f, 3.5f);
		camera.zoom = 1.4f;
	}

	void Render() override
	{
		// loop over every cluster in the world
		for (int c = 0; c < world->clusters.size(); c++)
		{
			// loop over every particle in each cluster
			for (Particle* p : world->clusters[c]->particles)
			{
				// Render each particle
				int segments = settings.circle_res;
				real rad = p->radius;
				vec2 pos = p->pos;
				colour col;
				colour outline_col;
				if (p->is_static)
				{
					col = palette.colours[colours.static_particle];
					outline_col = palette.colours[colours.static_particle_outline];
				}
				else
				{
					col = palette.colours[colours.particle];
					outline_col = palette.colours[colours.particle_outline];
				}
				if (p == world->clusters[1]->particles[49])
					col = palette.colours[Palette::JP_DARK_GREEN];
				if (p == world->clusters[1]->particles[53])
					col = palette.colours[Palette::JP_RED];
	
				// Render particle
				renderer.AddSolidCircle(pos, rad, segments, col, outline_col);
			}
			// loop over every spring in each cluster
			if (settings.enable_springs)
				for (Spring* s : world->clusters[c]->springs)
				{
					// Render each spring
					vec2 posA = s->particleA->pos;
					vec2 posB = s->particleB->pos;
					colour col = palette.colours[colours.spring];
					renderer.AddLine(posA, posB, col);
				}
		}
		
		// Render a circle around selected particle
		colour outline_col(1.0f, 1.0f, 1.0f, 1.0f);
		if (selected_particle != nullptr)
			renderer.AddCircle(selected_particle->pos, selected_particle->radius * 1.2f, settings.circle_res, outline_col);
		
		// Render chunks
		if (settings.render_chunks)
			for (auto& [key, particles] : world->chunks)
			{
				real x, y;
				world->HashKeyToChunkCoords(key, &x, &y);
		
				real left = x;
				real right = x + settings.chunk_scale;
				real top = y + settings.chunk_scale;
				real bottom = y;
				colour col(1.0f, 1.0f, 0.0f, 0.25f);
				colour out_col(0.3f, 0.3f, 0.0f, 1.00f);
				renderer.AddTriangle(vec2(left, top), vec2(right, top), vec2(left, bottom), col);
				renderer.AddTriangle(vec2(right, top), vec2(left, bottom), vec2(right, bottom), col);
				renderer.AddLine(vec2(left, top), vec2(right, top), out_col);
				renderer.AddLine(vec2(left, top), vec2(left, bottom), out_col);
				renderer.AddLine(vec2(right, bottom), vec2(right, top), out_col);
				renderer.AddLine(vec2(right, bottom), vec2(left, bottom), out_col);
			}
	}
};

#endif
