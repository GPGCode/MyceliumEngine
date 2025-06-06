// MyceliumEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>
#include <GL/glew.h>
#include "Registry.h"
#include "Component.h"
#include "MovementSystem.h"
#include "AgingSystem.h"
#include "TimeSystem.h"
#include "Renderer.h"
#include "RenderSystem.h"
#include "RotationSystem.h"
#include "DecaySystem.h"
#include <typeinfo>
#undef main

void PrintDecayStateTypeNames() {
    std::cout << "[File] DecayState typeid: " << typeid(DecayState).name() << std::endl;
}
//Initializing SDL for windowing
bool InitSDL(SDL_Window*& window, SDL_GLContext& context) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    // Request OpenGL 3.3 Core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("Mycelium Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing GLEW!\n";
        return false;
    }

    std::cout << "OpenGL initialized. Version: " << glGetString(GL_VERSION) << "\n";
    return true;
}




uint64_t now = SDL_GetPerformanceCounter();
uint64_t last = 0;
float deltaTime = 0.0f;


int main()
{

    
    // Initialize the renderer
    auto renderer = std::make_shared<Renderer>();
    if (!renderer->Init("Mycelium Engine", 800, 600)) return -1;

    // Create registry and systems
    auto registry = std::make_shared<Registry>();
    auto renderSystem = std::make_shared<RenderSystem>(renderer);
    auto timeSystem = std::make_shared<TimeSystem>();
    auto rotationSystem = std::make_shared<RotationSystem>();
    auto movementSystem = std::make_shared<MovementSystem>();
    auto agingSystem = std::make_shared<AgingSystem>();
    auto decaySystem = std::make_shared<DecaySystem>();

    renderSystem->SetRegistry(registry);
    renderer->SetRegistry(registry); // optional if needed by renderer
    movementSystem->SetRegistry(registry);
    rotationSystem->SetRegistry(registry);
    agingSystem->SetRegistry(registry);
    decaySystem->SetRegistry(registry);

    // Register component types
    registry->RegisterComponent<Renderable>();
    registry->RegisterComponent<Position>();
    registry->RegisterComponent<Color>();
    registry->RegisterComponent<Rotation>();
    registry->RegisterComponent<Age>();
    registry->RegisterComponent<Movement>();
    registry->RegisterComponent<DecayState>();
    std::type_index decayType(typeid(DecayState));
    std::cout << "[main] Registered DecayState: " << typeid(DecayState).name()
        << " | Hash: " << decayType.hash_code() << std::endl;

    // Create and setup an entity
    auto triangleOne = registry->CreateEntity();
    auto triangleTwo = registry->CreateEntity();
    auto triangleThree = registry->CreateEntity();

    registry->AddComponent(triangleOne, Position{ 0.0f,  0.3f });
    registry->AddComponent(triangleTwo, Position{ 0.3f,  0.0f });
    registry->AddComponent(triangleThree, Position{ 0.0f, -0.3f });
    registry->AddComponent(triangleOne, Renderable{ }); // orange
    registry->AddComponent(triangleTwo, Renderable{ }); // orange
    registry->AddComponent(triangleThree, Renderable{ }); // orange
    registry->AddComponent(triangleOne, Color{ 1.0f, 0.0f, 0.0f, 1.0f }); // Red
    registry->AddComponent(triangleTwo, Color{ 0.0f, 1.0f, 0.0f, 1.0f }); // Green
    registry->AddComponent(triangleThree, Color{ 0.0f, 0.0f, 1.0f, 1.0f }); // Blue
    registry->AddComponent(triangleOne, Rotation{ 0.2f });
    registry->AddComponent(triangleTwo, Rotation{ 0.7f });
    registry->AddComponent(triangleThree, Rotation{ 0.6f });
    registry->AddComponent(triangleOne, Movement{ 0.0f, 0.5f, 0.0f });
    registry->AddComponent(triangleTwo, Movement{ 0.0f, 0.2f, 90.0f });
    registry->AddComponent(triangleThree, Movement{ 0.0f, 2.0f, 45.0f });

    registry->AddComponent(triangleOne, DecayState{ 0.0f, 0.2f });
    registry->AddComponent(triangleThree, DecayState{ 0.0f, 0.2f });
    registry->AddComponent(triangleTwo, DecayState{ 0.0f, 0.2f });
    registry->AddComponent(triangleOne, Age{});
    registry->AddComponent(triangleTwo, Age{});
    registry->AddComponent(triangleThree, Age{});
    // Main loop
    while (renderer->IsRunning()) {
        last = now;
        now = SDL_GetPerformanceCounter();
        
        deltaTime = static_cast<float>(now - last) / SDL_GetPerformanceFrequency();
        timeSystem->Update(deltaTime);


        renderer->PollEvents();
        renderer->Clear();
        rotationSystem->Update(timeSystem->GetDeltaTime());
        movementSystem->Update(timeSystem->GetDeltaTime());
        agingSystem->Update(timeSystem->GetDeltaTime());
        decaySystem->Update(timeSystem->GetDeltaTime());
        renderSystem->Update();
        renderer->Present();
    }

    renderer->ShutDown();
    return 0;
}

