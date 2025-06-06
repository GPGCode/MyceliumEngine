
#include "MovementSystem.h"
#include "Component.h"
#include <cmath>
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

constexpr float dragFactor = 0.5f;
void MovementSystem::Update(float deltaTime) {
    // Iterate over all entities this system is interested in
    for (auto  entity : registry->GetEntitiesWith<Position, Movement>()) {

        auto& position = registry->GetComponent<Position>(entity.GetID());
        auto& movement = registry->GetComponent<Movement>(entity.GetID());
        movement.speed += movement.acceleration * deltaTime;
        movement.speed -= dragFactor * movement.speed * deltaTime;
        movement.speed = std::clamp(movement.speed, 0.0f, 10.0f);

        if (movement.useFacingDirection) {
            if (registry->HasComponent<Rotation>(entity.GetID())) {
                auto& rotation = registry->GetComponent<Rotation>(entity.GetID());
                movement.directionAngle = rotation.angle;
            }
        }
        float radians = glm::radians(movement.directionAngle);
        float dx = cos(radians) * movement.speed * deltaTime;
        float dy = sin(radians) * movement.speed * deltaTime;

        position.x += dx;
        position.y += dy;
    }
}