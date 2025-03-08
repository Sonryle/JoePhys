#ifndef JP_SCENE
#define JP_SCENE

typedef float real;

#include <JoePhys/World.hpp>

// Base struct for all scene structs. Every scene struct will
// inherit from this base Scene struct
struct Scene
{
	// constructors & destructors
	Scene();
	virtual ~Scene();

	// virtual functions (virtual means that it can be overwritten by structs inherited from this struct
	virtual void Render();
	virtual void ResetCamera();

	// functions
	void Step();

protected:
	
	// functions
	virtual void SetUpSceneColours() = 0;
	
	// our world struct
	World* world;
};

#endif
