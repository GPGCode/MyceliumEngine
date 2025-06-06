#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <string>
#include "Registry.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	bool Init(const std::string& title, int width, int height);
	void Clear();
	void DrawTriangle();
	void Present();
	void CleanUp();
	bool IsRunning() const;
	void PollEvents();
	void ShutDown();
	void SetRegistry(std::shared_ptr<Registry> reg);

private:
	SDL_Window* window = nullptr;
	SDL_GLContext glContext = nullptr;
	bool isRunning = true;
	GLuint VAO = 0, VBO = 0;
	GLuint shaderProgram = 0;
	std::shared_ptr<Registry> registry;

	bool InitShaders();
	bool InitTriangle();
};