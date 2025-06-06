#pragma once
#include "System.h"
#include <iostream>
#include "Registry.h"
#include "Renderable.h"
#include "Component.h"
#include <GL/glew.h>
#include "Renderer.h"
class Renderer;

class RenderSystem : public System {
public:
	RenderSystem(std::shared_ptr<Renderer> renderer);
	void Update();
	void SetRegistry(std::shared_ptr<Registry> reg);

private:
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<Registry> registry;
};