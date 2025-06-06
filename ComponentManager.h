#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include "ComponentArray.h"
#include "Common.h"
#include "IComponentArray.h"

class ComponentManager {
public:
	template<typename T>
	void RegisterComponent()
	{
		std::type_index typeIndex(typeid(T));

		assert(componentTypes.find(typeIndex) == componentTypes.end() && "Registering component type more than once.");

		componentTypes[typeIndex] = nextComponentType;
		componentArrays[typeIndex] = std::make_shared<ComponentArray<T>>();

		++nextComponentType;
	}

	template<typename T>
	ComponentType GetComponentType() {
		std::type_index typeIndex(typeid(T));

		assert(componentTypes.find(typeIndex) != componentTypes.end() && "Component not registered before use.");

		return componentTypes[typeIndex];
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(Entity::IDType entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(Entity entity) {
		for (auto const& pair : componentArrays) {
			pair.second->EntityDestroyed(entity.GetID());
		}
	}

	template<typename T>
	bool HasComponent(Entity::IDType entity) {
		return GetComponentArray<T>()->HasComponent(entity);
	}

private:
	std::unordered_map<std::type_index, ComponentType> componentTypes{};
	std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> componentArrays{};
	ComponentType nextComponentType{};

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray() {
		std::type_index typeIndex(typeid(T));

		assert(componentTypes.find(typeIndex) != componentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeIndex]);
	}

};