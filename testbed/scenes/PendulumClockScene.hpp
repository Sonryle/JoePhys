#ifndef PENDULUM_CLOCK_SCENE
#define PENDULUM_CLOCK_SCENE

// TEMPORARY FOR "stderr"
#include <cstdio>

#include "../Scenes.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"
#include "JoePhys/Particle.hpp"
#include "JoePhys/Spring.hpp"

struct PendulumClockScene : public Scene
{
	// constructors & destructors
	PendulumClockScene()
	{
		SetUpSceneColours();
		settings.Reset();
		world->Create(settings.simulation_hertz, settings.sub_steps, vec2(0.0f, -9.8f));
		settings.circle_res = 22;
		settings.enable_drag = 0;
		camera.zoom = 1.0f;
		camera.center.Set(0.0f, 0.0f);
		double PI = 3.141592653589;

		// Create an axel
		// ---------------
		
		Cluster* axel = new Cluster;
		const int axel_segments = 16;
		real axel_radius = 0.26f;
		vec2 axel_pos(0.0f, 1.0f);


		vec2 vel(0.0f, 0.0f);
		real rad = 0.1f;
		real elas = 0.5f;
		real mass = 1.0f;
		Particle* inside_p = new Particle(axel_pos, vel, elas, rad, mass, 1);
		axel->particles.push_back(inside_p);

		real rotation_increment = 2.0f * (float)PI / axel_segments;

		real sin_increment = sinf(rotation_increment);
		real cos_increment = cosf(rotation_increment);

		vec2 rotation(1.0f, 1.0f);
		vec2 point_pos(axel_pos + axel_radius * rotation);

		Particle* axel_parts[axel_segments];
		for (int i = 0; i < axel_segments; i++)
		{
			vec2 new_rotation;

			new_rotation.x = cos_increment * rotation.x - sin_increment * rotation.y;
			new_rotation.y = sin_increment * rotation.x + cos_increment * rotation.y;

			vec2 new_point_pos = axel_pos + axel_radius * new_rotation;

			// Create a particle at point pos
			vec2 pos(new_point_pos);
			vec2 vel(0.0f, 0.0f);
			real rad = 0.07f;
			real elas = 0.5f;
			real mass = (real)PI * rad * rad;
			Particle* p = new Particle(pos, vel, elas, rad, mass, 0);
			axel_parts[i] = p;
			axel->particles.push_back(p);

			// Create springs to connect particles
			real stiffness_insides = 1000.0f;
			real stiffness_outsides = 1500.0f;
			Spring* spring = new Spring(axel_parts[i], inside_p, length(axel_parts[i]->pos_in_meters - inside_p->pos_in_meters), stiffness_outsides);
			axel->springs.push_back(spring);
			if (i > 0)
			{
				Spring* s = new Spring(axel_parts[i], axel_parts[i-1], length(axel_parts[i]->pos_in_meters - axel_parts[i-1]->pos_in_meters), stiffness_outsides);
				axel->springs.push_back(s);
			}
			if (i == axel_segments - 1)
			{
				Spring* s = new Spring(axel_parts[i], axel_parts[0], length(axel_parts[i]->pos_in_meters - axel_parts[0]->pos_in_meters), stiffness_insides);
				axel->springs.push_back(s);
			}

			rotation = new_rotation;
			point_pos = new_point_pos;
		}

		// Create a pendulum arm & weight
		// ------------------------------

		Cluster* arm = new Cluster;

		real w_rad = 0.5f;
		real w_mass = PI * w_rad * w_rad;
		Particle* weight = new Particle(vec2(0.0f, -4.0f), vec2(1.0f, 0.0f), 0.5f, w_rad, w_mass, 0);
		arm->particles.push_back(weight);

		real stiffness = 1700.0f;
		real len = length(axel_parts[13]->pos_in_meters - weight->pos_in_meters);
		Spring* s = new Spring(weight, axel_parts[13], len, stiffness);
		real len2 = length(axel_parts[5]->pos_in_meters - weight->pos_in_meters);
		Spring* s2 = new Spring(weight, axel_parts[5], len2, stiffness);

		arm->springs.push_back(s);
		arm->springs.push_back(s2);

		// Create the hooks coming off of the axel
		// --------------------------------------

		Cluster* hooks = new Cluster;

		vec2 h1_pos(-0.9f, 0.65f);
		real h1_rad = 0.07f;
		real h1_elas = 0.5f;
		real h1_mass = PI * h1_rad * h1_rad;
		Particle* h1 = new Particle(h1_pos, vec2(0.0f, 0.0f), h1_elas, h1_rad, h1_mass, 0);
		hooks->particles.push_back(h1);

		real h1_stiffness = 1800.0f;
		real h1_len = length(axel_parts[6]->pos_in_meters - h1->pos_in_meters);
		Spring* h1_s = new Spring(h1, axel_parts[6], h1_len, h1_stiffness);
		hooks->springs.push_back(h1_s);

		h1_len = length(axel_parts[3]->pos_in_meters - h1->pos_in_meters);
		Spring* h1_s2 = new Spring(h1, axel_parts[3], h1_len, h1_stiffness);
		hooks->springs.push_back(h1_s2);


		// Add clusters to world
		// ---------------------

		world->clusters.push_back(axel);
		world->clusters.push_back(arm);
		world->clusters.push_back(hooks);

	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_RED;
		colours.spring = Palette::JP_YELLOW;
		colours.particle = Palette::JP_DARK_YELLOW;
		colours.particle_outline = Palette::JP_DARK_GRAY;
		colours.static_particle = Palette::JP_GRAY;
		colours.static_particle_outline = Palette::JP_DARK_GRAY;
	}
};

#endif
