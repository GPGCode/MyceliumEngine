#pragma once
struct Position {
	float x = 0.0f;
	float y = 0.0f;
};

struct Velocity {
	float dx = 0.0f;
	float dy = 0.0f;
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
	float age = 0.0;
	float agingRate = 1.0f;
};

struct EnvironmentalProperties {
	float moisture = 0.0f;     // 0 (dry) to 1 (soaked)
	float temperature = 20.0f; // in Celsius
	float lightExposure = 1.0f;        // 0 (dark) to 1 (full sun)
};
