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

		systemManager.EntitySignatureChanged(entity, signature);
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


private:
	EntityManager entityManager;
	ComponentManager componentManager;
	SystemManager systemManager;
};