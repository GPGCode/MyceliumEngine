#pragma once
struct Position {
	float x = 0.0f;
	float y = 0.0f;
};

struct Movement
{
	float speed;
	float acceleration;
	float directionAngle;
	bool useFacingDirection = true;
};

struct Rotation
{
	float angle = 0.0f;
};

struct Progression {
	float value = 0.0f;
	float rate = 0.0f;
	float min = 0.0f;
	float max = 1.0f;
};

struct EffectTrigger {
	float threshold = 1.0f;
};

struct Age {
	float age = 0.0f;
};

struct DecayState
{
	float level = 0.0f;
	float rate = 0.1f;
};

struct Color {
	float r, g, b, a;
};


struct EnvironmentalProperties {
	float moisture = 0.0f;     // 0 (dry) to 1 (soaked)
	float temperature = 20.0f; // in Celsius
	float lightExposure = 1.0f;        // 0 (dark) to 1 (full sun)
};
