#pragma once

#include "Entity.h"

class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity::IDType entity) = 0;
};