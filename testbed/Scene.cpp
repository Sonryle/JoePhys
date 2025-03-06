#include "Scene.hpp"

#include "JoePhys/Shape.hpp"
#include "JoePhys/Circle.hpp"

#include "Settings.hpp"
#include "Renderer.hpp"

void Scene::Render()
{
	// Get all PhysObjects in world
	std::vector<PhysObject*> objects = world.getPhysObjects();

	// loop over every PhysObject and add them to the renderer
	for (int i = 0; i < objects.size(); i++)
	{
		PhysObject* current_obj = objects[i];
		
		// If PhysObject is a circle, add that circle to the stack
		if (current_obj->shape->GetType() == Shape::eCircle)
		{
			const vec2 pos = current_obj->position;
			const Circle* circ = (Circle*)current_obj->shape;
			const colour fill_col = palette.colours[settings.scene_colours.circles];
			const colour outline_col = palette.colours[settings.scene_colours.circle_outlines];
			renderer.AddSolidCircle(pos, circ->radius, settings.circle_res, fill_col, outline_col);
		}
		else
		{
			fprintf(stderr, "You are trying to render a shape which does not exist yet lols sorryyyy!!\n");
		}
	}
}
