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
		for (auto entity : entities)
		{
			auto& entitiesAgeComponent = registry->GetComponent<Age>(entity);
			entitiesAgeComponent.age += deltaTime * entitiesAgeComponent.agingRate;
		}
	}

private:
	std::shared_ptr<Registry> registry;
};