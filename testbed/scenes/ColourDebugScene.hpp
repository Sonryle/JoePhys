#ifndef JP_COLOUR_DEBUG_SCENE
#define JP_COLOUR_DEBUG_SCENE

#include "../Scenes.hpp"
#include "../Colour.hpp"
#include "../Renderer.hpp"

struct ColourDebugScene : public Scene
{
	// constructors & destructors
	ColourDebugScene()
	{
		SetUpSceneColours();
		world = nullptr;
	}

	void Render() override
	{
		renderer.AddColourTest();
	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_DARK_GRAY;
	}
};

#endif
