#ifndef JP_TEST_SCENE_TWO
#define JP_TEST_SCENE_TWO

#include "JoePhys/Circle.hpp"
#include "JoePhys/PhysObject.hpp"

#include "../Scene.hpp"
#include "../Renderer.hpp"
#include "../Settings.hpp"

class TestSceneTwo : public Scene
{
public:
	TestSceneTwo()
	{
		world = World();
		ResetCamera();
		ResetSceneColours();

		// add a 15x15 grid of circles to the scene
		for (int x = -2; x < 3; x++)
			for (int y = -2; y < 3; y++)
			{
				Circle* c = new Circle(10.0f);
				vec2 pos(x * 20, y * 20);
				float mass = 1.0f;
				PhysObject* o = new PhysObject(c, pos, mass);
				world.AddPhysObject(o);
			}
	}
	~TestSceneTwo() {};
	
	void ResetCamera() override
	{
		camera.center.set(vec2(0.0f, 0.0f));
		camera.zoom = 1.0f;
	}
	void ResetSceneColours() override
	{
		settings.ResetSceneColours();
		settings.scene_colours.background = Palette::JP_GREEN;
		settings.scene_colours.circles = Palette::JP_RED;
		settings.scene_colours.circle_outlines = Palette::JP_DARK_GRAY;
	}

};

#endif
