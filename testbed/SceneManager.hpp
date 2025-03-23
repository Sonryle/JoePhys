#ifndef SCENE_MANAGER
#define SCENE_MANAGER

#include "Scenes.hpp"
#include "Settings.hpp"
#include "AllSceneHeaders.hpp"

struct SceneManager
{
	// Constructors & destructors
	SceneManager() : current_scene(nullptr), current_scene_number(0) {}
	~SceneManager() {}

	// functions
	void SwitchScene(int scene_number);
	
	// variables
	Scene* current_scene;
	unsigned int current_scene_number;
};

extern SceneManager scene_manager;

#endif
