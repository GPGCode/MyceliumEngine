
#include "MovementSystem.h"
#include "Component.h"
void MovementSystem::Update(float deltaTime) {
    // Iterate over all entities this system is interested in
    for (auto const& entity : entities) {
        auto& position = registry->GetComponent<Position>(entity);
        auto& velocity = registry->GetComponent<Velocity>(entity);

        position.x += velocity.dx * deltaTime;
        position.y += velocity.dy * deltaTime;
    }
}