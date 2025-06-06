#include "Renderer.h"
#include <iostream>
#include "Registry.h"
#include "Renderable.h"
#include "Component.h"
float vertices[] = {
	-0.5f, -.5f,
	0.5f, -0.5f,
	0.0f, 0.5f
};

const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec2 aPos;

uniform vec2 uOffset;
uniform float uRotation;

void main() {
	float s = sin(uRotation);
	float c = cos(uRotation);

	mat2 rotation = mat2(
        c, -s,
        s,  c
    );
	
	vec2 rotatedPos = rotation * aPos;
	gl_Position = vec4(rotatedPos + uOffset, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
uniform vec4 uColor;
out vec4 FragColor;
void main() {
    FragColor = uColor; // orange
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

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	GLint success;
	char infoLog[512];
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		false;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		false;
	}

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


	GLint offsetLoc = glGetUniformLocation(shaderProgram, "uOffset");
	GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
	GLint rotationLoc = glGetUniformLocation(shaderProgram, "uRotation");


	for (auto& entity : registry->GetEntitiesWith<Renderable, Position, Rotation>())
	{
		auto& pos = registry->GetComponent<Position>(entity.GetID());
		auto& color = registry->GetComponent<Color>(entity.GetID());
		auto& rot = registry->GetComponent<Rotation>(entity.GetID());

		//Add offset and colors to the Vertex/Frag shaders
		glUniform2f(offsetLoc, pos.x, pos.y);
		glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
		glUniform1f(rotationLoc, rot.angle);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
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

void Renderer::SetRegistry(std::shared_ptr<Registry> reg) {
	registry = reg;
}