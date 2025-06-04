#pragma once
#pragma once

#include <memory>
#include <vector>
#include "SystemManager.h"
#include "Registry.h"
#include "Components.h"  // Your Position and Velocity structs

class MovementSystem : public System {
public:
    void Update(float deltaTime);

    void SetRegistry(std::shared_ptr<Registry> registry) {
        this->registry = registry;
    }

private:
    std::shared_ptr<Registry> registry;
};