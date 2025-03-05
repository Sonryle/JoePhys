#include "JoePhys/World.hpp"

World::World()
{
	return;
}

void World::addPhysObject(PhysObject* obj)
{
	physObjects.push_back(obj);
}

void World::removePhysObject(PhysObject* obj)
{
	// loop over every object in the PhysObjects stack
	for (int i = 0; i < physObjects.size(); i++)
	{
		// If we find a match, delete it
		if (physObjects[i] == obj)
			physObjects.erase(physObjects.begin() + i);
	}
}

std::vector<PhysObject*> World::getPhysObjects()
{
	return physObjects;
}
