#ifndef JP_COLOUR_DEBUG_SCENE
#define JP_COLOUR_DEBUG_SCENE

#include "../Scene.hpp"
#include "../Settings.hpp"
#include "../Renderer.hpp"

struct ColourDebugScene : public Scene
{
	// constructors & destructors
	ColourDebugScene()
	{
		SetUpSceneColours();
	}

	void Render() override
	{
		renderer.AddColourTest();
	}

	void SetUpSceneColours() override
	{
		settings.scene_colours.background = Palette::JP_DARK_GRAY;
	}
};

#endif
