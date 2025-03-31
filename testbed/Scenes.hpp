#ifndef JP_SCENE
#define JP_SCENE

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <JoePhys/World.hpp>
#include "Colour.hpp"

typedef float real;

struct SceneColours
{
	SceneColours()
	{
		background = Palette::JP_AQUA;
		spring = Palette::JP_GREEN;
		particle = Palette::JP_DARK_GRAY;
		particle_outline = Palette::JP_DARK_GRAY;
		static_particle = Palette::JP_DARK_GREEN;
		static_particle_outline = Palette::JP_DARK_GRAY;
	}
	int background;
	int spring;
	int particle;
	int particle_outline;
	int static_particle;
	int static_particle_outline;
};

// Base struct for all scene structs. Every scene struct will
// inherit from this base Scene struct
struct Scene
{
	// constructors & destructors
	Scene();
	virtual ~Scene();

	// virtual functions (virtual means that it can be overwritten by structs inherited from this struct
	virtual void Render(GLFWwindow* window, vec2 cursor_pos);
	virtual void ResetCamera();
	virtual void SetUpScene();
	virtual void SetUpSceneSettings();

	// functions
	void Step();
	void AddStaticParticle(vec2 pos, real radius);
	void AddRepulsionForce(vec2 pos, real radius, real strength);
	void AddAttractionForce(vec2 pos, real radius, real strength);
	Particle* GetNearestParticle(vec2 pos);
	void MoveParticle(Particle* particle, vec2 pos);

	// SceneColours class
	SceneColours colours;

	// our world struct
	World* world;

protected:
	
	// functions
	virtual void SetUpSceneColours() = 0;
	
};

#endif
