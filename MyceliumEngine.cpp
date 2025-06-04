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
#undef main






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

int main()
{


//    SDL_Window* window = nullptr;
//    SDL_GLContext context;
//    if (!InitSDL(window, context)) {
//        return -1;
//    }
//
//    float vertices[] = {
//     0.0f,  0.5f,  // top
//    -0.5f, -0.5f,  // bottom left
//     0.5f, -0.5f   // bottom right
//    };
//
//    unsigned int VBO, VAO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // Vertex attribute pointer
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    const char* vertexShaderSource = R"(
//#version 330 core
//layout (location = 0) in vec2 aPos;
//
//void main()
//{
//    gl_Position = vec4(aPos, 0.0, 1.0);
//}
//)";
//
//    const char* fragmentShaderSource = R"(
//#version 330 core
//out vec4 FragColor;
//
//void main()
//{
//    FragColor = vec4(1.0, 0.5, 0.2, 1.0);  // orange
//}
//)";
//
//
//
//
//
//
//    //compiling shaders
//
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
//    glCompileShader(vertexShader);
//
//    // Check for errors (optional but helpful)
//    int success;
//    char infoLog[512];
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
//        std::cerr << "Vertex Shader Compilation Failed:\n" << infoLog << '\n';
//    }
//
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
//    glCompileShader(fragmentShader);
//
//    // Check for errors
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
//        std::cerr << "Fragment Shader Compilation Failed:\n" << infoLog << '\n';
//    }
//
//
//
//
//
//
//    //Linking shaders into a shader program.
//
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    // Check for linking errors
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
//        std::cerr << "Shader Program Linking Failed:\n" << infoLog << '\n';
//    }
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//
//
//
//
//    std::cout << "Welcome to the Mycelium Engine.\n";
//
//    //testing ECS
//
//    auto registry = std::make_shared<Registry>();
//
//    // Register components
//    registry->RegisterComponent<Position>();
//    registry->RegisterComponent<Velocity>();
//    registry->RegisterComponent<Age>();
//
//    // Create the MovementSystem and register it
//    auto movementSystem = registry->RegisterSystem<MovementSystem>();
//    auto agingSystem = registry->RegisterSystem<AgingSystem>();
//
//    // Set the system's signature: requires Position & Velocity components
//    Signature movementSignature;
//    movementSignature.set(registry->GetComponentType<Position>());
//    movementSignature.set(registry->GetComponentType<Velocity>());
//    registry->SetSystemSignature<MovementSystem>(movementSignature);
//
//    Signature agingSignature;
//    agingSignature.set(registry->GetComponentType<Age>());
//    registry->SetSystemSignature<AgingSystem>(agingSignature);
//
//    // Provide the registry pointer for component access
//    movementSystem->SetRegistry(registry);
//    agingSystem->SetRegistry(registry);
//
//    // Create an entity and add Position & Velocity
//    auto entity = registry->CreateEntity();
//    registry->AddComponent(entity, Position{ 0.0f, 0.0f });
//    registry->AddComponent(entity, Velocity{ 1.0f, 1.5f });
//    registry->AddComponent(entity, Age{ 0.0f, 1.0f });
//
//    bool isRunning = true;
//    SDL_Event event;
//    TimeSystem time;
//    time.SetTimeScale(1.0f);
//
//
//    
//
//    while (isRunning) {
//        // Handle events
//        while (SDL_PollEvent(&event)) {
//            if (event.type == SDL_QUIT) {
//                isRunning = false;
//            }
//        }
//
//        float deltaTime = 0.016f;
//
//        // Update your ECS stuff
//        movementSystem->Update(deltaTime);
//        agingSystem->Update(deltaTime);
//        time.Update(deltaTime);
//
//        auto& pos = registry->GetComponent<Position>(entity);
//        auto& age = registry->GetComponent<Age>(entity);
//        std::cout << "Age = " << age.age << " seconds | SimTime = " << time.GetTotalTime()
//            << "s | Position = (" << pos.x << ", " << pos.y << ")\n";
//
//        // Clear once, draw once, swap once
//        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glUseProgram(shaderProgram);
//        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//
//        SDL_GL_SwapWindow(window);
//
//    }
//
//
//    SDL_GL_DeleteContext(context);
//    SDL_DestroyWindow(window);
//    SDL_Quit();
Renderer renderer;
if (!renderer.Init("Mycelium Engine", 800, 600)) return -1;

while (renderer.IsRunning()) {
    renderer.PollEvents();
    renderer.Clear();
    renderer.DrawTriangle();
    renderer.Present();
}

renderer.ShutDown();
    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
