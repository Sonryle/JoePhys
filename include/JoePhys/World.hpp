#ifndef JP_WORLD
#define JP_WORLD

#include <cstdio>	// for "stderr" file path constant
#include <vector>
#include "PhysObject.hpp"

class World
{
public:
	World();

	void AddPhysObject(PhysObject* obj);
	void RemovePhysObject(PhysObject* obj);
	void RemoveAllPhysObjects();

	std::vector<PhysObject*> getPhysObjects();

private:
	std::vector<PhysObject*> physObjects;
};

#endif
