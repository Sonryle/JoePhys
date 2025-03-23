#include "SceneManager.hpp"
#include "glad/glad.h"

void SceneManager::SwitchScene(int scene_number)
{
	delete current_scene;
	current_scene_number = scene_number;
	switch (scene_number)
	{
	case 0:
		current_scene = new CollisionScene();
		break;
	case 1:
		current_scene = new PinballScene();
		break;
	case 2:
		current_scene = new SquishySquareScene();
		break;
	case 3:
		current_scene = new SquishyRectangleScene();
		break;
	case 4:
		current_scene = new ColourDebugScene();
		break;
	default:
		SwitchScene(0);
		break;
	}
	colour bg = palette.colours[current_scene->colours.background];
	glClearColor(bg.r, bg.g, bg.b, bg.a);
}

SceneManager scene_manager;
