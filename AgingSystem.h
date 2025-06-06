#pragma once
#include "Component.h"
#include "Registry.h"
class AgingSystem : public System{
public:
	void SetRegistry(std::shared_ptr<Registry> reg)
	{
		registry = reg;
	}

	void Update(float deltaTime)
	{
		for (auto entity : registry->GetEntitiesWith<Age>())
		{
			auto& entitiesAgeComponent = registry->GetComponent<Age>(entity.GetID());
			entitiesAgeComponent.age += deltaTime;
		}
	}

private:
	std::shared_ptr<Registry> registry;
};