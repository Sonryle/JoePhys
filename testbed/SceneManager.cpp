#include "SceneManager.hpp"
#include "scenes/NewtonsCradleScene.hpp"

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
		current_scene = new NewtonsCradleScene();
		break;
	case 5:
		current_scene = new ClothScene();
		break;
	case 6:
		current_scene = new RopeScene();
		break;
	case 7:
		current_scene = new ColourDebugScene();
		break;
	default:
		SwitchScene(0);
		break;
	}
}

SceneManager scene_manager;
