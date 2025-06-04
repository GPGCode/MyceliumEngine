#pragma once

#include <unordered_map>
#include <memory>
#include <bitset>
#include <set>
#include <typeindex>
#include "Common.h"
#include "System.h"


class SystemManager {
public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem() {
		const std::type_index typeName = typeid(T);
		assert(systems.find(typeName) == systems.end() && "Systems already registered");

		auto system = std::make_shared<T>();
		//make sure systems are inheriting from Systems publicly. :eyes:
		systems[typeName] = std::static_pointer_cast<System>(system);
		return system;
	}

	template<typename T>
	void SetSignature(Signature signature)
	{
		const std::type_index typeName = typeid(T);
		assert(systems.find(typeName) != systems.end() && "System not registered!");

		signatures[typeName] = signature;
	}

	
	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : systems) {
			auto const& system = pair.second;
			system->entities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity::IDType entity, Signature entitySignature)
	{
		for (const auto& pair : systems)
		{
			const auto& typeIndex = pair.first;
			const auto& system = pair.second;

			const auto& systemSignature = signatures[typeIndex];
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->entities.insert(entity);
			}
			else {
				system->entities.erase(entity);
			}
		}
	}
private:
	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
	std::unordered_map<std::type_index, Signature> signatures;
};