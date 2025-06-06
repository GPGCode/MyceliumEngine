#pragma once

#include "System.h"
#include "Component.h"
#include "Registry.h"
#include <iostream>
#include <typeinfo>

void PrintDecayStateTypeName() {
	std::cout << "[File] DecayState typeid: " << typeid(DecayState).name() << std::endl;
}
class DecaySystem : public System{
public:
	void SetRegistry(std::shared_ptr<Registry> reg)
	{
		registry = reg;
	}

	void Update(float deltaTime)
	{
		for (auto entity : registry->GetEntitiesWith<DecayState>())
		{
			auto& decay = registry->GetComponent<DecayState>(entity.GetID());
			decay.level += decay.rate * deltaTime;

			if (decay.level > 1.0f)
			{
				decay.level = 1.0f;
			}

			if (registry->HasComponent<Color>(entity.GetID())) {
				auto& color = registry->GetComponent<Color>(entity.GetID());

				// Fade alpha based on decay level
				color.a = 1.0f - decay.level;
			}
		}
	}

private:
	std::shared_ptr<Registry> registry;
};