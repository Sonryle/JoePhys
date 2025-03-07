#ifndef JP_COLOUR_TEST_SCENE
#define JP_COLOUR_TEST_SCENE

#include "JoePhys/Circle.hpp"
#include "JoePhys/PhysObject.hpp"

#include "../Scene.hpp"
#include "../Renderer.hpp"
#include "../Settings.hpp"

class ColourTestScene : public Scene
{
public:
	ColourTestScene()
	{
		world = World();
		ResetCamera();
		ResetSceneColours();
	}
	~ColourTestScene() {};
	
	void ResetCamera() override
	{
		camera.center.set(vec2(0.0f, 0.0f));
		camera.zoom = 1.0f;
	}

	void ResetSceneColours() override
	{
		settings.ResetSceneColours();
		settings.scene_colours.background = Palette::JP_DARK_GRAY;
	}

	void Render() override
	{
		renderer.AddColourTest();
	}

};

#endif
