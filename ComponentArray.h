#pragma once
#include <array>
#include <unordered_map>
#include <cassert>
#include "Entity.h"
#include "Common.h"
#include "IComponentArray.h"

template<typename T>
class ComponentArray : public IComponentArray {
public:
	void InsertData(Entity entity, const T& component)
	{
		assert(entityToIndexMap.find(entity.GetID()) == entityToIndexMap.end() && "Component already added to entity");

		size_t newIndex = size;
		entityToIndexMap[entity.GetID()] = newIndex;
		indexToEntityMap[newIndex] = entity.GetID();
		componentArray[newIndex] = component;
		++size;
	}

	void RemoveData(Entity::IDType entity)
	{
		assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Removing non-existent component");

		size_t indexOfRemovedEntity = entityToIndexMap[entity];
		size_t indexOfLastElement = size - 1;
		componentArray[indexOfRemovedEntity] = componentArray[indexOfLastElement];

		EntityID entityOfLastElement = indexToEntityMap[indexOfLastElement];
		entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		entityToIndexMap.erase(entity);
		indexToEntityMap.erase(indexOfLastElement);

		--size;
	}

	T& GetData(Entity::IDType entity)
	{
		assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Retrieving non-existent component");
		return componentArray[entityToIndexMap[entity]];
	}

	void EntityDestroyed(Entity::IDType entity)
	{
		if (entityToIndexMap.find(entity) != entityToIndexMap.end())
		{
			RemoveData(entity);
		}
	}

	bool HasComponent(Entity::IDType entity) {
		return entityToIndexMap.find(entity) != entityToIndexMap.end();
	}

private:
	std::array<T, MAX_ENTITIES> componentArray{};
	std::unordered_map<Entity::IDType, size_t> entityToIndexMap{};
	std::unordered_map<size_t, Entity::IDType> indexToEntityMap{};
	size_t size = 0;
};