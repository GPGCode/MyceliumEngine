#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <string>


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

private:
	SDL_Window* window = nullptr;
	SDL_GLContext glContext = nullptr;
	bool isRunning = true;

	GLuint VAO = 0, VBO = 0;
	GLuint shaderProgram = 0;

	bool InitShaders();
	bool InitTriangle();
};