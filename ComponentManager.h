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
		const char* typeName = typeid(T).name();

		assert(componentTypes.find(typeName) == componentTypes.end() && "Registering component type more than once.");

		componentTypes[typeName] = nextComponentType;
		componentArrays[typeName] = std::make_shared<ComponentArray<T>>();

		++nextComponentType;
	}

	template<typename T>
	ComponentType GetComponentType() {
		const char* typeName = typeid(T).name();

		assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered before use.");

		return componentTypes[typeName];
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
			pair.second->EntityDestroyed(entity);
		}
	}

private:
	std::unordered_map<const char*, ComponentType> componentTypes{};
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays{};
	ComponentType nextComponentType{};

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray() {
		const char* typeName = typeid(T).name();

		assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
	}

};