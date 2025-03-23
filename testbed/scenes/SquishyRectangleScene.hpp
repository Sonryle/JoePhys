#ifndef SQUISHY_RECTANGLE_SCENE
#define SQUISHY_RECTANGLE_SCENE

// TEMPORARY FOR "stderr"
#include <cstdio>

#include "../Scenes.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"
#include "JoePhys/Particle.hpp"
#include "JoePhys/Spring.hpp"

struct SquishyRectangleScene : public Scene
{
	// constructors & destructors
	SquishyRectangleScene()
	{
		SetUpSceneColours();
		world = new World(settings.simulation_hertz, settings.sub_steps, vec2(0.0f, -9.8f));
		settings.circle_res = 20;
		camera.zoom = 1.0f;
		camera.center.Set(4.0f, -4.0f);
		double PI = 3.141592653589;
		
		// Create a floor
		// --------------

		Cluster* floor = new Cluster;
		for (int x = -20; x < 30; x++)
		{
			real radius = 0.025f;
			vec2 pos(x * radius * 2.0f, -2.5f - x * 0.02f);
			vec2 vel(0.0f, 0.0f);
			real elasticity = 0.5f;
			real mass = 0.0f;
			Particle* p = new Particle(pos, vel, elasticity, radius, mass);

			// Add particle to floor cluster
			/* floor->particles.push_back(p); */
		}
		for (int x = -20; x < 30; x++)
		{
			real radius = 0.025f;
			vec2 pos((x * radius * 2.0f) + 4.0f, -4.5f + x * 0.02f);
			vec2 vel(0.0f, 0.0f);
			real elasticity = 0.5f;
			real mass = 0.0f;
			Particle* p = new Particle(pos, vel, elasticity, radius, mass);

			// Add particle to floor cluster
			floor->particles.push_back(p);
		}
		for (int x = -40; x < 40; x++)
		{
			real radius = 0.025f;
	 		vec2 pos((x * radius * 2.0f) + 0.5f, -5.5f - sin((x / 20.0f) + 26.5f) * 0.8f);
			vec2 vel(0.0f, 0.0f);
			real elasticity = 0.5f;
			real mass = 0.0f;
			Particle* p = new Particle(pos, vel, elasticity, radius, mass);

			// Add particle to floor cluster
			floor->particles.push_back(p);
		}

		// Create a horizontal Rectangle
		// -----------------------------
	
		Cluster* horiz_rect = new Cluster;

		// Create a 7x14 grid of particles, connected by springs
		Particle* hp[14][7];
		for (int x = 0; x < 14; x++)
		{
			for (int y = 0; y < 7; y++)
			{
				// Add Particles
				vec2 pos((x * 0.2f) - 3 * 0.2f, (y * 0.2f) - 5.0f);
				vec2 vel(0.0f, 0.0f);
				real elas = 0.5f;
				real rad = 0.05f;
				real mass = (real)PI * rad * rad;
				hp[x][y] = new Particle(pos, vel, elas, rad, mass);
				horiz_rect->particles.push_back(hp[x][y]);

				// Add Springs
				real stiffness = 100.0f;
				if (x > 0)
				{
					Spring* s = new Spring(hp[x][y], hp[x-1][y], 0.2f, stiffness);
					horiz_rect->springs.push_back(s);
				}
				if (y > 0)
				{
					Spring* s = new Spring(hp[x][y], hp[x][y-1], 0.2f, stiffness);
					horiz_rect->springs.push_back(s);
				}
				if (y > 0 && x > 0)
				{
					Spring* s = new Spring(hp[x-1][y], hp[x][y-1], sqrt(0.08f), stiffness);
					Spring* s2 = new Spring(hp[x][y], hp[x-1][y-1], sqrt(0.08f), stiffness);
					horiz_rect->springs.push_back(s);
					horiz_rect->springs.push_back(s2);
				}
			}
		}

		// Create a circle
		// ---------------
		
		Cluster* circ = new Cluster;
		const int circ_segments = 17;
		real circ_radius = 0.3f;
		vec2 circ_pos(0.0f, 1.3f);


		vec2 vel(0.0f, 0.0f);
		real rad = 0.1f;
		real elas = 0.5f;
		real mass = (real)PI * rad * rad;
		Particle* inside_p = new Particle(circ_pos, vel, elas, rad, mass);
		circ->particles.push_back(inside_p);

		real rotation_increment = 2.0f * (float)PI / circ_segments;

		real sin_increment = sinf(rotation_increment);
		real cos_increment = cosf(rotation_increment);

		vec2 rotation(1.0f, 1.0f);
		vec2 point_pos(circ_pos + circ_radius * rotation);

		Particle* circ_parts[circ_segments];
		for (int i = 0; i < circ_segments; i++)
		{
			vec2 new_rotation;

			new_rotation.x = cos_increment * rotation.x - sin_increment * rotation.y;
			new_rotation.y = sin_increment * rotation.x + cos_increment * rotation.y;

			vec2 new_point_pos = circ_pos + circ_radius * new_rotation;

			// Create a particle at point pos
			vec2 pos(new_point_pos);
			vec2 vel(-1.0f, 0.0f);
			real rad = 0.07f;
			real elas = 0.5f;
			real mass = (real)PI * rad * rad;
			Particle* p = new Particle(pos, vel, elas, rad, mass);
			circ_parts[i] = p;
			circ->particles.push_back(p);

			// Create springs to connect particles
			real stiffness = 80.0f;
			Spring* spring = new Spring(circ_parts[i], inside_p, length(circ_parts[i]->pos_in_meters - inside_p->pos_in_meters), stiffness);
			circ->springs.push_back(spring);
			if (i > 0)
			{
				Spring* s = new Spring(circ_parts[i], circ_parts[i-1], length(circ_parts[i]->pos_in_meters - circ_parts[i-1]->pos_in_meters), stiffness);
				circ->springs.push_back(s);
			}
			if (i == circ_segments - 1)
			{
				Spring* s = new Spring(circ_parts[i], circ_parts[0], length(circ_parts[i]->pos_in_meters - circ_parts[0]->pos_in_meters), stiffness);
				circ->springs.push_back(s);
			}

			rotation = new_rotation;
			point_pos = new_point_pos;
		}

		// Add clusters to world
		world->clusters.push_back(floor);
		world->clusters.push_back(horiz_rect);
		/* world->clusters.push_back(circ); */
	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_DARK_GREEN;
		colours.spring = Palette::JP_YELLOW;
		colours.particle = Palette::JP_DARK_YELLOW;
		colours.particle_outline = Palette::JP_DARK_GRAY;
		colours.static_particle = Palette::JP_GRAY;
		colours.static_particle_outline = Palette::JP_DARK_GRAY;
	}
};

#endif
