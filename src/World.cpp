#include "JoePhys/World.hpp"

World::World()
{
	return;
}

void World::AddPhysObject(PhysObject* obj)
{
	physObjects.push_back(obj);
}

void World::RemovePhysObject(PhysObject* obj)
{
	// loop over every object in the PhysObjects stack
	for (int i = 0; i < physObjects.size(); i++)
	{
		// If we find a match, delete it
		if (physObjects[i] == obj)
		{
			delete obj;
			physObjects.erase(physObjects.begin() + i);
		}
	}
}

void World::RemoveAllPhysObjects()
{
	// loop backwards over every object in the PhysObjects stack,
	// deleting it and then removing it's pointer from the stack.
	for (int i = physObjects.size() - 1; i >= 0; i--)
	{
		delete physObjects[physObjects.size() - 1];
		physObjects.pop_back();
	}
}

std::vector<PhysObject*> World::getPhysObjects()
{
	return physObjects;
}
