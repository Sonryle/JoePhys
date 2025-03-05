#ifndef JP_TEST_SCENE
#define JP_TEST_SCENE

#include "JoePhys/Circle.hpp"
#include "JoePhys/PhysObject.hpp"

#include "../Scene.hpp"
#include "../Renderer.hpp"
#include "../Settings.hpp"

class TestScene : public Scene
{
public:
	TestScene()
	{
		world = World();
		ResetCamera();
		ResetSceneColours();

		// add a 15x15 grid of circles to the scene
		for (int x = 0; x < 15; x++)
			for (int y = 0; y < 15; y++)
			{
				Circle* c = new Circle(10.0f);
				vec2 pos(x * 20, y * 20);
				float mass = 1.0f;
				PhysObject* o = new PhysObject(c, pos, mass);
				world.AddPhysObject(o);
			}
	}
	~TestScene() {};
	
	void ResetCamera() override
	{
		camera.center.set(vec2(0.0f, 0.0f));
		camera.zoom = 1.0f;
	}
	void ResetSceneColours() override
	{
		settings.scene_colours.background = &palette.aqua;
	}

};

#endif
