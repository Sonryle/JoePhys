#include "glm/detail/type_vec.hpp"
#include "shapes.hpp"
#include <cmath>
#include <particle_manager.hpp>

ParticleManager::ParticleManager(int simulation_hertz) : spawner(&particle_stack)
{
	// Set up time step & simulation hertz
	this->simulation_hertz = simulation_hertz;
	time_step_in_seconds = 1.0f / (float)simulation_hertz;

	// Set up spawner
	spawner.setParticlesPerSecond(1);
	spawner.setMaxParticleCount(2);
	/* spawner.setParticleElasticity(1.0f); */
	spawner.setParticleRadius(34.25f);
	spawner.setParticleRadius(100.0f);
	/* spawner.setParticleColour(glm::vec4(0.3f, 0.55f, 0.17f, 1.0f)); */
	spawner.setParticleColour(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	spawner.setParticleInitialVelocity(glm::vec2(1000.0f, 180.0f));
	spawner.setPosition(glm::vec2(-350.0f, 400.0f));

	// Set up default constraint vars
	constraint_pos = glm::vec2(0.0f, 0.0f);
	constraint_x_scale = 1000;
	constraint_y_scale = 1000;

	// Set up gravity
	/* gravity = glm::vec2(0.0f, -9806.65f); */
	gravity = glm::vec2(0.0f, 0.0f);

	return;
}

void ParticleManager::update()
{
	spawner.update(time_step_in_seconds);
	updateParticles();
	solveCollisions();
	constrainParticles();

	return;
}

void ParticleManager::setConstraint(glm::vec2 pos, float x_scale, float y_scale)
{
	constraint_pos = pos;
	constraint_x_scale = x_scale;
	constraint_y_scale = y_scale;
}

void ParticleManager::updateParticles()
{
	// loop over every particle and update their positions
	for (int i = 0; i < (int)particle_stack.size(); i++)
	{
		particle_stack[i]->accelerate(gravity);
		particle_stack[i]->update(time_step_in_seconds);
	}
}

void ParticleManager::constrainParticles()
{
	// loop over every particle
	for (int i = 0; i < (int)particle_stack.size(); i++)
	{
		float top_wall = constraint_pos.y + (constraint_y_scale / 2);
		float bottom_wall = constraint_pos.y - (constraint_y_scale / 2);
		float left_wall = constraint_pos.x - (constraint_x_scale / 2);
		float right_wall = constraint_pos.x + (constraint_x_scale / 2);

		Particle* particle = particle_stack[i];

		if (particle->position.y + particle->radius > top_wall)
		{
			// get distance between new_pos.y and old_pos.y
			float velocity = particle->position.y - particle->old_position.y;

			// flip edge of circle around the bottom wall
			float edge_of_particle = particle->position.y + particle->radius;
			particle->position -= glm::vec2(0.0f, 2 * (edge_of_particle - top_wall));

			// apply old velocity to particle (but inverted). Also multiply velocity by elasticity
			particle->old_position = glm::vec2(particle->old_position.x, particle->position.y + velocity * particle->elasticity);
		}

		if (particle->position.y - particle->radius < bottom_wall)
		{
			// get distance between new_pos.y and old_pos.y
			float velocity = particle->position.y - particle->old_position.y;

			// flip edge of circle around the bottom wall
			float edge_of_particle = particle->position.y - particle->radius;
			particle->position -= glm::vec2(0.0f, 2 * (edge_of_particle - bottom_wall));

			// apply old velocity to particle (but inverted). Also multiply velocity by elasticity
			particle->old_position = glm::vec2(particle->old_position.x, particle->position.y + velocity * particle->elasticity);
		}

		if (particle->position.x - particle->radius < left_wall)
		{
			// get distance between new_pos.y and old_pos.y
			float velocity = particle->position.x - particle->old_position.x;

			// flip edge of circle around the bottom wall
			float edge_of_particle = particle->position.x - particle->radius;
			particle->position -= glm::vec2(2 * (edge_of_particle - left_wall), 0.0f);

			// apply old velocity to particle (but inverted). Also multiply velocity by elasticity
			particle->old_position = glm::vec2(particle->position.x + velocity * particle->elasticity, particle->old_position.y);
		}

		if (particle->position.x + particle->radius > right_wall)
		{
			// get distance between new_pos.y and old_pos.y
			float velocity = particle->position.x - particle->old_position.x;

			// flip edge of circle around the bottom wall
			float edge_of_particle = particle->position.x + particle->radius;
			particle->position -= glm::vec2(2 * (edge_of_particle - right_wall), 0.0f);

			// apply old velocity to particle (but inverted). Also multiply velocity by elasticity
			particle->old_position = glm::vec2(particle->position.x + velocity * particle->elasticity, particle->old_position.y);
		}


		// update the position of the circle which reprisents our particle
		particle->circle.position = particle->position;
	}
}

void ParticleManager::solveCollisions()
{
	const int particle_count = (int)particle_stack.size();

	// loop over every particle
	for (int n = 0; n < particle_count; n++)
	{
		// compair this particle with every other particle in the particle stack
		Particle* particle_one = particle_stack[n];
		for (int i = n+1; i < particle_count; i++)
		{
			Particle* particle_two = particle_stack[i];
			const glm::vec2 initial_collision_axis = particle_one->position - particle_two->position;
			const float initial_distance = length(initial_collision_axis);

			// if particles overlap
			if (initial_distance < particle_one->radius + particle_two->radius)
			{
				// Oh no, the particles collided???
				
				// store our particle's initial velocities
				const glm::vec2 particle_one_initial_velocity = particle_one->position - particle_one->old_position;
				const glm::vec2 particle_two_initial_velocity = particle_two->position - particle_two->old_position;

				// ------------------------------------------------------------
				// step 1: move particles backwards along their velocities
				// 	   until they reach the point where they first collided
				// ------------------------------------------------------------
				
				// the distance apart that we want our particles to reach
				const float target_distance = particle_one->radius + particle_two->radius;

				const float dx = particle_one->position.x - particle_two->position.x;
				const float dy = particle_one->position.y - particle_two->position.y;

				const float vx = particle_one_initial_velocity.x - particle_two_initial_velocity.x;
				const float vy = particle_one_initial_velocity.y - particle_two_initial_velocity.y;

				const float a = (vx * vx) + (vy * vy);
				const float b = (2 * dx * vx) + (2 * dy * vy);
				const float c = (dx * dx) + (dy * dy) - (target_distance * target_distance);

				// get the offset to move them by
				const float offset = (-b - sqrt((b * b) - (4.0f * a * c))) / (2.0f * a);

				// update their positions by that offset
				particle_one->position += particle_one_initial_velocity * (offset);
				particle_two->position += particle_two_initial_velocity * (offset);

				// cancel any velocity (we will be updating the velocity later)
				particle_one->old_position = particle_one->position;
				particle_two->old_position = particle_two->position;


				// --------------------------------------------------------------
				// step 2: Find the velocities of each particle perpendicular to
				//         their axis of collision and swap them around (only the
				//         perpendicular velocities get swapped)
				// --------------------------------------------------------------

				const glm::vec2 axis_of_collision = normalize(particle_one->position - particle_two->position);
				
				// Project the velocities of the particles onto the axis of collision
				const glm::vec2 p1_projection = axis_of_collision * (dot(particle_one_initial_velocity, axis_of_collision) / dot(axis_of_collision, axis_of_collision));
				const glm::vec2 p2_projection = axis_of_collision * (dot(particle_two_initial_velocity, axis_of_collision) / dot(axis_of_collision, axis_of_collision));

				// Get the 
				const glm::vec2 p1_perpendicular = particle_one_initial_velocity - p1_projection;
				const glm::vec2 p2_perpendicular = particle_two_initial_velocity - p2_projection;

				// Create the new velocities
				const glm::vec2 particle_one_new_velocity = p2_projection + p1_perpendicular;
				const glm::vec2 particle_two_new_velocity = p1_projection + p2_perpendicular;

				/* const glm::vec2 p1_projection_line = (dot()) */

						// Create a debug line along the axis of collision
						Line* aoc_line = new Line();
						aoc_line->start_position = particle_two->position - axis_of_collision * glm::vec2(150);
						aoc_line->end_position = particle_one->position + axis_of_collision * glm::vec2(150);
						aoc_line->thickness = 3;
						aoc_line->layer = 2;
						aoc_line->colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

						// Create a debug line for the velocity of particle 1
						Line* p1v_line = new Line();
						p1v_line->start_position = particle_one->position;
						p1v_line->end_position = particle_one->position + particle_one_initial_velocity;
						p1v_line->thickness = 10;
						p1v_line->layer = 2;
						p1v_line->colour = glm::vec4(6.0f, 0.0f, 0.0f, 1.0f);
	
						// Create a debug line for the projection of particle 1
						Line* p1prog_line = new Line();
						p1prog_line->start_position = particle_one->position;
						p1prog_line->end_position = particle_one->position + p1_projection;
						p1prog_line->thickness = 10;
						p1prog_line->layer = 2;
						p1prog_line->colour = glm::vec4(1.0f, 0.4f, 0.4f, 1.0f);

						// Create a debug line for the vector perpendicular to the axis of collision for particle 1
						Line* p1perp_line = new Line();
						p1perp_line->start_position = particle_one->position + p1_projection;
						p1perp_line->end_position = particle_one->position + p1_projection + p1_perpendicular;
						p1perp_line->thickness = 10;
						p1perp_line->layer = 2;
						p1perp_line->colour = glm::vec4(1.0f, 0.4f, 0.4f, 1.0f);

						// Create a debug line to visualize the INHERITED projected velocity from the othe particle
						Line* p1inherited_line = new Line();
						p1inherited_line->start_position = particle_one->position;
						p1inherited_line->end_position = particle_one->position + p2_projection;
						p1inherited_line->thickness = 10;
						p1inherited_line->layer = 2;
						p1inherited_line->colour = glm::vec4(0.6f, 0.6f, 1.0f, 1.0f);

						// Create a debug line for the NEW velocity of particle 1
						Line* p1n_line = new Line();
						p1n_line->start_position = particle_one->position;
						p1n_line->end_position = particle_one->position + particle_one_new_velocity;
						p1n_line->thickness = 10;
						p1n_line->layer = 2;
						p1n_line->colour = glm::vec4(0.93f, 0.74f, 0.01f, 1.0f);
	
						// ----------------------------------------------------------------------------------------------

						// Create a debug line for the velocity of particle 2
						Line* p2v_line = new Line();
						p2v_line->start_position = particle_two->position;
						p2v_line->end_position = particle_two->position + particle_two_initial_velocity;
						p2v_line->thickness = 10;
						p2v_line->layer = 2;
						p2v_line->colour = glm::vec4(0.2f, 0.2f, 0.5f, 1.0f);
	
						// Create a debug line for the projection of particle 2
						Line* p2prog_line = new Line();
						p2prog_line->start_position = particle_two->position;
						p2prog_line->end_position = particle_two->position + p2_projection;
						p2prog_line->thickness = 10;
						p2prog_line->layer = 2;
						p2prog_line->colour = glm::vec4(0.6f, 0.6f, 1.0f, 1.0f);

						// Create a debug line for the vector perpendicular to the axis of collision for particle 2
						Line* p2perp_line = new Line();
						p2perp_line->start_position = particle_two->position + p2_projection;
						p2perp_line->end_position = particle_two->position + p2_projection + p2_perpendicular;
						p2perp_line->thickness = 10;
						p2perp_line->layer = 2;
						p2perp_line->colour = glm::vec4(0.6f, 0.6f, 1.0f, 1.0f);

						// Create a debug line to visualize the INHERITED projected velocity from the other particle
						Line* p2inherited_line = new Line();
						p2inherited_line->start_position = particle_two->position;
						p2inherited_line->end_position = particle_two->position + p1_projection;
						p2inherited_line->thickness = 10;
						p2inherited_line->layer = 2;
						p2inherited_line->colour = glm::vec4(1.0f, 0.4f, 0.4f, 1.0f);

						// Create a debug line for the NEW velocity of particle 2
						Line* p2n_line = new Line();
						p2n_line->start_position = particle_two->position;
						p2n_line->end_position = particle_two->position + particle_two_new_velocity;
						p2n_line->thickness = 10;
						p2n_line->layer = 2;
						p2n_line->colour = glm::vec4(0.93f, 0.74f, 0.01f, 1.0f);
	
						// Render all debug lines
						temp_line_stack.push_back(aoc_line);
						temp_line_stack.push_back(p1prog_line);
						temp_line_stack.push_back(p1perp_line);
						temp_line_stack.push_back(p1inherited_line);
						temp_line_stack.push_back(p1v_line);
						temp_line_stack.push_back(p1n_line);
						temp_line_stack.push_back(p2prog_line);
						temp_line_stack.push_back(p2perp_line);
						temp_line_stack.push_back(p2inherited_line);
						temp_line_stack.push_back(p2v_line);
						temp_line_stack.push_back(p2n_line);


				// ------------------------------------------------------------
				// step 3: Shift particles along their new velocity by the same
				//         amount that we offset them by in step 1
				// ------------------------------------------------------------

				/* // move particles by the offset amount */
				/* particle_one->position += particle_one_new_velocity * offset; */
				/* particle_two->position += particle_two_new_velocity * offset; */

				/* // also update old_pos so that the velocity isnt affected */
				/* particle_one->old_position += particle_one_new_velocity * offset; */
				/* particle_two->old_position += particle_two_new_velocity * offset; */
			}
		}
	}
}
