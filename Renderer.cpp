#include "Renderer.h"
#include <iostream>

float vertices[] = {
	-0.5f, -.05f,
	0.5f, -0.5f,
	0.0f, 0.5f
};

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0); // orange
}
)";

Renderer::Renderer() {}
Renderer::~Renderer() {
	ShutDown();
}

bool Renderer::Init(const std::string& title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
	std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!window)
	{
		std::cerr << "Window could not be created! SDL_Error: " << "\n";
		return false;
	}

	glContext = SDL_GL_CreateContext(window);
	if (!glContext)
	{
		std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
		return false;
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) 
	{
		std::cerr << "Failed to initalize GLEW\n";
		return false;
	}

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

	return InitShaders() && InitTriangle();
}

bool Renderer::InitShaders() {
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

bool Renderer::InitTriangle() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void Renderer::Clear() {
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawTriangle() {
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::Present() {
	SDL_GL_SwapWindow(window);
}

void Renderer::PollEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			isRunning = false;
		}
	}
}

bool Renderer::IsRunning() const {
	return isRunning;
}

void Renderer::CleanUp()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Renderer::ShutDown() {
	CleanUp();
}