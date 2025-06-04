#pragma once

class TimeSystem {
public:
	void Update(float realDeltaTime)
	{
		deltaTime = realDeltaTime * timeScale;
		totalTime += deltaTime;
	}

	float GetDeltaTime() const { return deltaTime; }
	float GetTotalTime() const { return totalTime; }
	
	void SetTimeScale(float scale) { timeScale = scale; }
	float GetTimeScale() const { return timeScale; }

private:
	float timeScale = 1.0f;
	float deltaTime = 0.0f;
	float totalTime = 0.0f;
};