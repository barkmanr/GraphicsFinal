#ifndef Game_Time
#define Game_Time

#include "StandardIncludes.h"

class GameTime : public Singleton<GameTime>
{
public:
	GameTime();
	void Initialize();
	void Update();

	int FrameCount() { return frameCount; }
	int Fps() { return fps; }
	double DeltaTime() { return deltaTime; }
	double StartTime() { return startTime; }
	double CurrentTime() { return currentTime; }

private:
	int frameCount;
	double deltaTime;
	double startTime;
	double currentTime;

	int fps;
	int fpsCounter;
	double lastTime;
};

#endif Game_Time

