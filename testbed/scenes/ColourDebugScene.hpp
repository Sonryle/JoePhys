#ifndef JP_COLOUR_DEBUG_SCENE
#define JP_COLOUR_DEBUG_SCENE

#include "../Scenes.hpp"
#include "../Colour.hpp"
#include "../Renderer.hpp"
#include "../Settings.hpp"

struct ColourDebugScene : public Scene
{
	// constructors & destructors
	ColourDebugScene()
	{
		SetUpSceneColours();
		settings.Reset();
		world = nullptr;
	}

	void Render(GLFWwindow* window, vec2 cursor_pos) override
	{
		renderer.AddColourTest();
	}

	void SetUpSceneColours() override
	{
		colours.background = Palette::JP_DARK_GRAY;
	}

	void SetUpSceneSettings() override
	{
		settings.Reset();
		camera.center.Set(0.0f, 0.0f);
		camera.zoom = 1.0f;
	}
};

#endif
