#pragma once
#include <set>
#include "Entity.h"

class System
{
public:
	std::set<Entity::IDType> entities;

	virtual ~System() = default;
};