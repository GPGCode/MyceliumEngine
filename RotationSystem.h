#pragma once
#include "System.h"
#include "Registry.h"
#include "Component.h"


class RotationSystem : public System {
public:

	void SetRegistry(std::shared_ptr<Registry> registry) {
		this->registry = registry;
	}

	void Update(float deltaTime)
	{
		for (auto entity : registry->GetEntitiesWith<Rotation>()) {

			auto& rotation = registry->GetComponent<Rotation>(entity.GetID());
			rotation.angle += 45.0f * deltaTime / 2;

			if(rotation.angle >= 360.0f)
			{
				rotation.angle -= 360.0f;
			}
		}
	}
private:
	std::shared_ptr<Registry> registry;
};