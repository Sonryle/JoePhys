#ifndef JP_SCENE
#define JP_SCENE

#include "JoePhys/World.hpp"

// A base class which every scene class will be derived from
class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

	// Functions which will be changed by Scenes classes
	virtual void ResetCamera() = 0;
	virtual void ResetSceneColours() = 0;

	// Functions which are defined in the main Scene class.
	// (Other scene classes derived from this one will not
	//  edit these functions)
	void Render();

protected:
	World world;
};

#endif
