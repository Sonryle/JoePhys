#ifndef SCENE_MANAGER
#define SCENE_MANAGER

#include "Scenes.hpp"
#include "Settings.hpp"
#include "AllSceneHeaders.hpp"

struct SceneManager
{
	// Constructors & destructors
	SceneManager() : scene(nullptr), scene_number(-1) {}
	~SceneManager() {}

	// functions
	void SwitchScene(int scene_number);
	
	// variables
	Scene* scene;
	unsigned int scene_number;
};

extern SceneManager sm;

#endif
