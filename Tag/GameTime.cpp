#include "GameTime.h"

GameTime::GameTime()
{
	frameCount = 0;
	deltaTime = 0.0;
	startTime = 0.0;
	currentTime = 0.0;
	fps = 0;
	fpsCounter = 0;
	lastTime = 0.0;
}

void GameTime::Initialize()
{
	startTime = glfwGetTime();
}

void GameTime::Update()
{
	currentTime = glfwGetTime();
	fpsCounter++;
	if (currentTime - lastTime >= 1.0)
	{
		fps = fpsCounter;
		fpsCounter = 0;
		lastTime += 1.0;
	}
	deltaTime = currentTime - startTime;
	startTime = currentTime;
}
