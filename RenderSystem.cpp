#include "RenderSystem.h"
#include <iostream>
#include "Registry.h"
#include "Component.h"

RenderSystem::RenderSystem(std::shared_ptr<Renderer> renderer)
	: renderer(renderer) {}

void RenderSystem::SetRegistry(std::shared_ptr<Registry> reg) {
	registry = reg;  // inherited from System base class
}

void RenderSystem::Update()
{
	// Use ECS to get relevant entities
	auto entities = registry->GetEntitiesWith<Renderable, Position>();

	for (auto& entity : entities) {
		auto& pos = registry->GetComponent<Position>(entity.GetID());

		// Optional: apply transform here via uniform
		// For now: draw the same triangle for every entity
		renderer->DrawTriangle();
	}
}