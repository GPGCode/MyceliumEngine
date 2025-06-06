#pragma once
#include <vector>
#include "Entity.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
class Registry {
public:
	Entity CreateEntity() {
		return entityManager.CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		entityManager.DestroyEntity(entity);
		componentManager.EntityDestroyed(entity);
		systemManager.EntityDestroyed(entity);
	}

	template<typename T>
	void RegisterComponent()
	{
		componentManager.RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		componentManager.AddComponent<T>(entity, component);

		auto signature = entityManager.GetSignature(entity);
		signature.set(componentManager.GetComponentType<T>(), true);
		entityManager.SetSignature(entity, signature);

		systemManager.EntitySignatureChanged(entity.GetID(), signature);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		componentManager.RemoveComponent<T>(entity);

		auto signature = entityManager.GetSignature(entity);
		signature.set(componentManager.GetComponentType<T>(), false);
		entityManager.SetSignature(entity, signature);

		systemManager.EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& GetComponent(Entity::IDType entity)
	{
		return componentManager.GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		return componentManager.GetComponentType<T>();
	}

	template<typename T>
	std::shared_ptr<T> RegisterSystem() {
		return systemManager.RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		systemManager.SetSignature<T>(signature);
	}

	template<typename... ComponentTypes>
	std::vector<Entity> GetEntitiesWith()
	{
		Signature desiredSignature;
		(desiredSignature.set(GetComponentType<ComponentTypes>()), ...);

		std::vector<Entity> result;
		for (Entity::IDType id = 0; id < MAX_ENTITIES; ++id)
		{
			Entity entity(id);
			Signature entitySignature = entityManager.GetSignature(entity);
			if ((entitySignature & desiredSignature) == desiredSignature) {
				result.push_back(entity);
			}
		}
		return result;
	}

	template<typename T>
	bool HasComponent(Entity::IDType entity) {
		return componentManager.HasComponent<T>(entity);
	}

private:
	EntityManager entityManager;
	ComponentManager componentManager;
	SystemManager systemManager;
};