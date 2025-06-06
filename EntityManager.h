#pragma once
#include "Entity.h"
#include "Common.h"
#include <queue>
#include <bitset>
#include <cassert>
#include <array>
using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager {
private:
	std::queue<Entity::IDType> availableIDs;
	std::array<Signature, MAX_ENTITIES> signatures;
	std::size_t livingEntityCount = 0;

public:
	EntityManager()
	{
		for (Entity::IDType id = 0; id < MAX_ENTITIES; ++id)
		{
			availableIDs.push(id);
		}
	}
	
	Entity CreateEntity()
	{
		assert(livingEntityCount < MAX_ENTITIES && "Reach entity limit.");
		Entity::IDType id = availableIDs.front();
		availableIDs.pop();
		++livingEntityCount;
		return Entity(id);
	}

	void DestroyEntity(Entity entity)
	{
		assert(entity.GetID() < MAX_ENTITIES && "Invalid entity ID");
		signatures[entity.GetID()].reset();
		availableIDs.push(entity.GetID());
		--livingEntityCount;
	}

	void SetSignature(Entity entity, Signature signature)
	{
		signatures[entity.GetID()] = signature;
	}

	Signature GetSignature(Entity entity) const
	{
		return signatures[entity.GetID()];
	}
};